/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "mosquito_demand.h"

using namespace UniSim;

namespace rvf {

MosquitoDemand::MosquitoDemand(Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Parameter<double>("sexRatio", &sexRatio, 0.5, this, "Proportion of females");
    new Parameter<double>("density", &density, 100., this, "Density of adults");
    new Parameter<double>("demandRate", &demandRate, 0.2, this, "Demanded number of bloodmeals per female mosquito per day");
    new Variable<double>("value", &value, this, "Calculated demand for bloodmeals");
}

void MosquitoDemand::reset() {
    value = 0;
}

void MosquitoDemand::update() {
    value = density*sexRatio*demandRate;
}

} //namespace
