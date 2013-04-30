/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include "greenhouse.h"

using namespace UniSim;

namespace vg {
	
Greenhouse::Greenhouse(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("width", &width, 50., this, "Width (m)");
    new Parameter<double>("length", &length, 50., this, "Length (m)");
    new Parameter<double>("height", &height, 4., this, "Height (m)");
    new Parameter<double>("roofRatio", &roofRatio, 1., this, "Roof/Side wall ratio (-)");
    new Parameter<double>("sideRatio", &sideRatio, 1., this, "Side ratio ? (-)");

    new Parameter<double>("windowLength", &windowLength, 2., this, "Window length (m)");
    new Parameter<double>("windowHeight", &windowHeight, 0.825, this, "Window height (m)");
    new Parameter<double>("fractionWindows", &fractionWindows, 0.078, this, "Fraction windows [0;1]");
    new Parameter<double>("leakageVentilation", &leakageVentilation, 0.5, this, "Smaller values for better insulation");
    new Parameter<double>("alphaVentilationMax", &alphaVentilationMax, 44., this, "Max. ventilation opening (degrees)");
    new Parameter<double>("latcov", &latcov, 35., this, "Lat cover (degrees)");
    new Parameter<double>("Kcover", &Kcover, 7.9, this, "K value of greenhouse cover (W/m2/K)");
    new Parameter<double>("floorHec", &floorHec, 6, this, "Heat capacity of the floor (?)");


    new Variable<double>("volume", &volume, this, "Volume (m3)");
    new Variable<double>("groundArea", &groundArea, this, "Area of ground covered (m2)");
    new Variable<double>("coverArea", &coverArea, this, "Area of greenshouse cover (m2)");
}

void Greenhouse::reset() {
    double areaA = width*height;
    double areaB = areaA/4;
    double areaC = areaB;
    double areaTot = areaA + areaB + areaC;

    volume = areaTot*length;
    groundArea = width*length;
    coverArea = height*length*2 + height*width*2 + length*height/3.*2.; // Approx. cover area
}


} //namespace

