/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/output.h>
#include <usbase/output_variable.h>
#include "usbase/parameter.h"
#include "lake_environment.h"
#include "net_production.h"
#include "oxygen_concentration.h"
#include "math.h"

using namespace UniSim;


namespace lakeoxygen {

NetProduction::NetProduction(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    //From Lake_environment
    new Parameter<double>("K", &K, 1., this, "Reaeration Constant estimated from stochastic sampling [m/day]");
    new Parameter<double>("Ksys", &Ksys, 0.22, this, "Reaeration Constant for lemming system [m/d]");
    new Parameter<double>("Kd", &Kd, 1.75, this, "Light Attenuation coefficient [-]");
    new Parameter<double>("Unit", &Unit, 0.375, this, "Converts between Oxygen and Carbon as Net production unit");
    new Variable<double>("DO_OBS_SAT_PCT", &DO_OBS_SAT_PCT, this, "observed dissolved oxygen concentration as percentage of saturation");
    new Variable<double>("Kwind", &Kwind, this, "Reaeration coefficent estimated from wind speed [m/day]");
    new Variable<double>("Net_Prod", &Net_Prod, this, "Net community production (from stochastic sampling [O2(or C)/m2/day])");
    new Variable<double>("Net_Prod_wind", &Net_Prod_wind, this, "Net community production (from wind speed [O2(or C)/m2/day]) ");

    new Parameter<int>("Layers", &Layers, 3, this, "Number of layers to subdivide the water column vertically [-]");
    new Parameter<double>("MaxDepth", &MaxDepth, 15., this, "Description");
    new Variable<double>("Layerheight", &Layerheight, this, "Layerheight [m]");
    new Variable<double>("Sum_stochastic", &Sum_stochastic, this, "Calculated light at a cirtain depth [µmol/m2/day]");
     new Variable<double>("Sum_wind", &Sum_wind, this, "Calculated light at a cirtain depth [µmol/m2/day]");
      new Variable<double>("Light", &Light, this, "Calculated light at a cirtain depth [µmol/m2/day]");

}
void NetProduction::initialize() {
    CreateLayers();  //Was moved from the constructor to the initialize to ensure that the xml file is written.

    weather = seekOne<Model*>("weather");
    calendar = seekOne<Model*>("calendar");
    lakeEnvironments = seekMany<LakeEnvironment*>("*");
    OxygenConcentration = seekOne<Model*>("OxygenConcentration");


    //LakeEnvironment = seekOne<Model*>("LakeEnvironment");
}


void NetProduction::reset() {
                Sum_stochastic =0;
                Sum_wind =0;
        }

void NetProduction::update() {
    /*Pulls the variables from the models the do calculations*/
    double DOsatConc = OxygenConcentration->pullValue<double>("DOsatConc");
    double OBS_DO1 = weather->pullValue<double>("OBS_DO");
    double DLI = weather->pullValue<double>("OBS_DLI");
    double OBS_Wind = weather->pullValue<double>("OBS_Wind");
    double Daylength = weather->pullValue<double>("Daylength");



    /*Ensures that calculations is not zero*/
    if      (DOsatConc==0) DO_OBS_SAT_PCT=100;
    else
            DO_OBS_SAT_PCT =  (OBS_DO1/DOsatConc)*100;
             Kwind=((0.728*pow(OBS_Wind,0.5)) -0.317*OBS_Wind + (0.0372*pow(OBS_Wind,2)));

    /*Defines the Reaeration coefficient if it should be calculated from wind speed or if it is measures directly for the lake and held constant*/
    /*Ku10(Reaeration constant based on wind speed in 10 m height) final unit [m/d]*/
    if      (K==0)Ku10 = (0.2*OBS_Wind)/Ksys;
    else
            Ku10 = K/Ksys;

    /*Changes the unit calculation between Carbon units or Oxygen units*/
    if      (Unit==0)Unit=1;
    else    Unit=0.375;

    /*Calculates the Net primary production from the Oxygen saturation, DLI and Daylength*/
    /*The Net Primary Production is corrected for units and for average depth of the lake.*/
    /*The unit of the Net Primary production is [g Unit/m3/day]*/
    Net_Prod = ((-0.787 + 0.014*DO_OBS_SAT_PCT + 0.022*DLI -0.081*Daylength)*Ku10)*Unit;
    Net_Prod_wind = ((-0.787 + 0.014*DO_OBS_SAT_PCT + 0.022*DLI -0.081*Daylength)*(Kwind/Ksys))*Unit;



    QDate today = calendar->pullValue<QDate>("date");
    if (today.day() == 1 && today.month()==1) {
            Sum_stochastic =0;
            Sum_wind =0;
     }
            Sum_stochastic += Net_Prod;
            Sum_wind += Net_Prod_wind;




    for (int i = 0; i < lakeEnvironments.size(); ++i) {
        double Light = lakeEnvironments[i]->pullValue<double>("Light");


    }

}
void NetProduction::CreateLayers(){
    for (int i=0; i < Layers; ++i){
        QString Name="Layer"+QString::number(i+1);
        Model *Layer= new LakeEnvironment(Name,this);  //New creates it in the Heap
        Layer->deepInitialize();

        Parameter<double> *AverageDepth = Layer->seekOneChild<Parameter<double>*>("AvgDepth");
        PullVariable<double> *light = Layer->seekOneChild<PullVariable<double>*>("Light");
        addOutput(i, light);

        double Layerheight = MaxDepth/Layers - i;
        AverageDepth->setValue(Layerheight);
    }

    }


/*
void NetProduction::CreateLayers(){

  for (int i=0; i < Layers; ++i){

    QString Name="Layer"+QString::number(i+1);
    Model *Layer= new LakeEnvironment(Name,this);
    Layer->deepInitialize();  // *** Tilføjet ***
    Parameter<double> *AverageDepth = Layer->seekOneChild<Parameter<double>*>("AvgDepth");



    double Layerheight;
           Layerheight = MaxDepth/Layers - i;


    AverageDepth->setValue(Layerheight);

}*/


void NetProduction::addOutput(int layer, PullVariable<double> *light){
    QString label, axis, summary;
    Output *parent;

    label = "Light in Layer " + QString::number(layer+1);
    axis = "y";
    summary = "";
    parent = seekOne<Output*>("LightPlot"); // name of output plot in XML file

    new OutputVariable(label, axis, summary, light, parent);
}

} //namespace

