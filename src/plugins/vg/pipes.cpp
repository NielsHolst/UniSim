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
    new Variable<double>("heatTransfer", &heatTransfer, this, "Heat transfer of all pipes (W/m2)");
    new Variable<double>("heatEnergy", &heatEnergy, this, "Energy used for heating by all pipes (J/m2)");
}

void Pipes::initialize() {
    pipes = seekChildren<Pipe*>("*");
}

void Pipes::reset() {
    heatTransfer = heatEnergy = 0.;
}

void Pipes::update() {
    heatTransfer = heatEnergy = 0.;
    for (int i = 0; i < pipes.size(); ++i) {
        heatTransfer += pipes[i]->pullValue<double>("heatTransfer");
        heatEnergy += pipes[i]->pullValue<double>("heatEnergy");
    }
}


} //namespace

