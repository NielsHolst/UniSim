/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "mosquito_demand.h"
#include "publish.h"

using namespace UniSim;

namespace rvf {

PUBLISH(MosquitoDemand)

MosquitoDemand::MosquitoDemand(Identifier name, QObject *parent)
    : Model(name, parent)
{
    Input(double, sexRatio, 0.5);   // Proportion of females
    Input(double, density, 100.);   // Density of adults
    Input(double, demandRate, 0.2); // Demanded number of bloodmeals per female mosquito per day
    Output(double, value);          // Calculated demand for bloodmeals
}

void MosquitoDemand::reset() {
    value = 0;
}

void MosquitoDemand::update() {
    value = density*sexRatio*demandRate;
}

} //namespace
