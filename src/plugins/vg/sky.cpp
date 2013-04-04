/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "general.h"
#include "sky.h"

using namespace UniSim;

namespace vg {
	
Sky::Sky(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("Tair", &Tair, 27., this, "Air temperature (oC)");
    new Parameter<double>("rh", &rh, 80., this, "Relative humidity (%)");
    new Parameter<double>("slope", &slope, 0.00577, this, "Emissivity as a linear function of Tdew");
    new Parameter<double>("intercept", &intercept, 0.735, this, "Emissivity as a linear function of Tdew");
    new Variable<double>("temperature", &temperature, this, "Sky temperature");
    new Variable<double>("emissivity", &emissivity, this, "Sky emissivity");
}

void Sky::initialize() {
    Model *records = seekOneSibling<Model*>("records");
    recordedTsky = records->peekValuePtr<double>("Tsky");
}

void Sky::reset() {
    update();
}

void Sky::update() {
    double Tdew = vg::Tdew(Tair, rh);
    emissivity = intercept + slope*Tdew;
    if (recordedTsky)
        temperature = *recordedTsky;
    else {
        double f = pow(emissivity*(Tair + T0), 4);
        temperature = pow(f,0.25) - T0;
    }
}


} //namespace

