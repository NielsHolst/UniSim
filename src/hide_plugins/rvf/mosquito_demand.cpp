/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

#include "mosquito_demand.h"

using namespace UniSim;

namespace rvf {

MosquitoDemand::MosquitoDemand(Identifier name, QObject *parent)
     : Model(name, parent)
{
    new Parameter<double>("demandPerFemale", &demandPerFemale, 0.2, this, "Blood meals per day");
    new Parameter<double>("populationDensity", &populationDensity, 0., this, "Mosquitoes per square km");
    new Parameter<double>("sexRatio", &sexRatio, 0.5, this, "Ratio of females, between 0 and 1");
    new Variable<double>("value", &value,this, "Total demand of the population");
}
void MosquitoDemand::reset() {
    value = 0.;
}
void MosquitoDemand::update() {
    value = demandPerFemale * populationDensity * sexRatio;
}
} //namespace
