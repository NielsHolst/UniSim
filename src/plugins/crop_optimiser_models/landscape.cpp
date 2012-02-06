/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include "farm.h"
#include "landscape.h"
#include "crop.h"

using namespace UniSim;


namespace crop_optimiser {

Landscape::Landscape(UniSim::Identifier name, QObject *parent)
        : Model(name, parent)
{  


}

void Landscape::initialize(){
    //createFarms();



}//initialize

//void Landscape::createFarms() {

///**Creates all farms and a list of pointers to farms. Reads an input text file with
//farms data (ID, farm type, soil type, business/private and saves each farm's name (ID).*/

//    int n = cropAreas->numRows();
//    for (int i=0; i < n; ++i) {
//        QString id = cropAreas->key(i);
//        Farm *farm = new Farm(id, this);

//        QList<Identifier>cropNames=cropAreas->columnNames();//returns QList with strings/now-identifiers - crop names
//        QMap<QString, double> areas;
//        for(int j=0; j<cropNames.size(); j++){
//            QString name=cropNames[j].key();
//            //areas.insert(name, cropAreas->stringValue(id, name).toDouble()); //reads a string for farm with id number for a given crop and then transform the string to double
//            areas.insert(name, cropAreas->value<double>(id, name)); //AM 07.12.11
//        }

//        farm->setInitialCrops(areas);
//        farm->setCropParameters(cropParameters3);
//        farm->setLivestockParameters(livestockParameters);
//        farm->setLivestockNumbers(livestockNumbers);
//        farm->setWinterMaxValues (winterMaxValues);
//        farm->setPriceNt (priceNt);
//        farm->setPriceFU (priceFU);
//        farm->setMethod_fert (method_fert);

//        QString farmType1=farmsData->value<QString>(id, "FarmType");
//        farm->setFarmType(farmType1);
//        QString soilType1=farmsData->value<QString>(id, "SoilType");
//        farm->setSoilType(soilType1);

//        if(farmType1=="Pig"){
//            farm->setMinFodder(20);
//        }
//        else if(farmType1=="Cattle"){
//            farm->setMinFodder(35);
//        }

//       // farm->setFarmID(id); //instead just a line in farm's constructor


//        farm->deepInitialize();  // initializes farm and any models inside it
//        farms.append(farm);
//    }
//}


void Landscape::amend() { //called just once! after the xml is open

    farms=seekChildren<Farm*>("*"); //moved from initialize

    QList<Crop*>crops=farms[0]->seekChildren<Crop*>("*");
    int n = crops.size();
    cropTotals.resize(n);
    for (int i = 0; i < n; ++i) {
        QString name = crops[i]->id().label();
        double *value = &cropTotals[i];
        new PullVariable<double>(name, value, this, "desc");
    }
}

void Landscape::reset() {

      cropTotals.fill(0.);
}

void Landscape::update() {


    for(int i=0; i<farms.size(); i++){
        QList<Crop*>crops=farms[i]->seekChildren<Crop*>("*");

        Q_ASSERT(crops.size()==cropTotals.size()); //if this is false - programme will stop

        for(int j=0; j<crops.size(); j++){
            double area=crops[j]->pullVariable<double>("Area_ha");
            cropTotals[j]+=area;
        }
    }
}


} //namespace

