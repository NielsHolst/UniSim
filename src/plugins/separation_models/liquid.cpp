/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/exception.h>
#include <usbase/pull_variable.h>
#include "liquid.h"

using namespace UniSim;


namespace separation {

Liquid::Liquid(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
        new PullVariable<double>("TAN", &TAN, this, "TAN in slurry (kg)");
        new Parameter<double>("TANmol", &TANmol, 0.26, this, "mol of TAN - assumed that 1 kg slurry is equal to 1 liter (mol/liter");
        new Parameter<double>("pH", &pH, 7.8, this, "pH in slurry");
        new Parameter<double>("rA", &rA, 20, this, "Description");
        new Parameter<int>("area", &area, 1000, this, "Surface area og slurry tank");
        new Parameter<QString>("coverlist", &coverlist, QString ("((non 0) (crust 100) (plastic 110))"), this, "List of different covers of slurry tank");
        new Parameter<QString> ("cover", &cover, QString ("non"), this, "Default cover");
        new PullVariable<double>("Kn", &Kn, this, "Ammonium-ammonia equilibrium constant");
        new PullVariable<double>("Kh", &Kh, this, "Henry's law coefficient");
        new PullVariable<double>("NH3aq", &NH3aq, this, "ammonia in solution");
        new PullVariable<double>("NH3g", &NH3g, this, "Gaseous ammonia in equlibrium (mol/L)");
        new PullVariable<double>("NH3emis", &NH3emis, this, "Emission of ammonia pr m2 (kg/m2/day)");
        new PushVariable<double> ("TANin", &TANin, this, "TAN from the separtion");
        new PushVariable<double> ("Totalemis", &Totalemis, this, "Total ammonia emission pr day");

}

void Liquid::initialize(){
    weather=seekOne<Model*>("weather");
}

void Liquid::reset() {
    TAN=0;
    TANin=0;
    update();
//    if (cover == "Non")
//      rC=0 ; /*1*/
//     else if (cover == "Crust")
//       rC=100; /*0.22*/
//    else if (cover== "Plastic")
//       rC=110;/*0.17*/
//    else if (cover=="tent")
//       rC=200; /*0.11*/
//    else if (cover== "concrete")
//        rC=200; /*0.11*/

    QMap<QString,double> decoded;
       decoded = decodeList<QString,double>(coverlist);
       if (!decoded.contains(cover)) throw Exception ("cover '" +cover+ "' not valid");
       rC=decoded [cover];



}

void Liquid::update() {
    TAN += TANin;
        TANin=0;
    double K_temp=weather ->pullVariable <double> ("temp")+ 273.15;/*Temperatur i Kelvin*/
    Kn = pow(10, -0.09018-(2729.92/K_temp)); /*No dimentions*/
    TANmol = TAN/14; /* mol/L - assumed that 1 kg slurry is equal to 1 liter*/
    Kh = pow(10,-1.69+(1477.7/K_temp)); /*No dimentions*/
    NH3aq = TANmol/((1+(pow(10,-pH)/Kn)));/* mol/L*/
    NH3g = NH3aq/Kh; /*mol/L*/
    NH3emis = (14*NH3g/(rA+rC))*60*60*24; /* Kg NH3-N/m2/day*/
    Totalemis = NH3emis*area;
    TAN-= NH3emis;




}

} //namespace

