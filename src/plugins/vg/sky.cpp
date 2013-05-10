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
    addParameterRef<double>(Name(Tair), "indoors/temperature[air]");
    addParameterRef<double>(Name(rh), "indoors/humidity[rh]");
    addParameter<double>(Name(slope), 0.00577, "Emissivity as a linear function of Tdew");
    addParameter<double>(Name(intercept), 0.735, "Emissivity as a linear function of Tdew");
    addVariable<double>(Name(temperature), "Sky temperature");
    addVariable<double>(Name(emissivity), "Sky emissivity");
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

