/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "lamp_base.h"

using namespace UniSim;

namespace vg {
	
LampBase::LampBase(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<bool>("switchedOn", &switchedOn, false, this, "Lamps switched on?");
    new Parameter<double>("capacity", &capacity, 40., this, "Capacity of installed assimilation lamps (W/m2)");
    new Variable<double>("convectiveHeat", &convectiveHeat, this, "Convective heat");
    new Variable<double>("bnLight", &bnLight, this, "Bn produced by the lamps reaching the crop");
    new Variable<double>("rnLight", &rnLight, this, "Short-wave radiation that reaches the crop");
    new Variable<double>("par", &par, this, "PAR that reaches the crop");
    new Variable<double>("energyUse", &energyUse, this, "Energy use, including the ballast");
}

void LampBase::reset() {
    setToZero();
}

void LampBase::setToZero() {
    convectiveHeat = bnLight = rnLight = par = energyUse = 0.;
}

void LampBase::update() {
    if (switchedOn)
        calculate();
    else
        setToZero();
}



} //namespace

