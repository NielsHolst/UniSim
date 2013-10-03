/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "pipe.h"
#include "pipes.h"

using namespace UniSim;

namespace vg {
	
Pipes::Pipes(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameterRef<double>(Name(timeStepsSecs), "calendar[timeStepSecs]");
    addVariable<double>(Name(heatFlux), "Total heat transfered from all pipes (W/m2)");
    addVariable<double>(Name(energyUsed), "Accumulated energy used for heating (MJ/m2)");
}

void Pipes::initialize() {
    heatFluxes.clear();
    QList<Pipe*> pipes = seekChildren<Pipe*>("*");
    for (int i = 0; i < pipes.size(); ++i) {
        heatFluxes << pipes.at(i)->pullValuePtr<double>("heatFlux");
    }
}

void Pipes::reset() {
    heatFlux = energyUsed = 0.;
}

void Pipes::update() {
    heatFlux = 0.;
    for (int i = 0; i < heatFluxes.size(); ++i) {
        heatFlux += *heatFluxes.at(i);
    }
    energyUsed += heatFlux*timeStepsSecs/1e6;
}

} //namespace

