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
    addParameter<double>(Name(width), 50., "Width (m)");
    addParameter<double>(Name(length), 50., "Length (m)");
    addParameter<double>(Name(height), 4., "Height (m)");
    addParameter<double>(Name(roofRatio), 1., "Roof/Side wall ratio (-)");
    addParameter<double>(Name(sideRatio), 1., "Side ratio ? (-)");

    addParameter<double>(Name(windowLength), 2., "Window length (m)");
    addParameter<double>(Name(windowHeight), 0.825, "Window height (m)");
    addParameter<double>(Name(fractionWindows), 0.078, "Fraction windows [0;1]");
    addParameter<double>(Name(leakageVentilation), 0.5, "Smaller values for better insulation");
    addParameter<double>(Name(alphaVentilationMax), 44., "Max. ventilation opening (degrees)");
    addParameter<double>(Name(latcov), 35., "Lat cover (degrees)");
    addParameter<double>(Name(Kcover), 7.9, "K value of greenhouse cover (W/m2/K)");
    addParameter<double>(Name(floorHec), 6, "Heat capacity of the floor (?)");


    addVariable<double>(Name(volume), "Volume (m3)");
    addVariable<double>(Name(groundArea), "Area of ground covered (m2)");
    addVariable<double>(Name(coverArea), "Area of greenshouse cover (m2)");
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

