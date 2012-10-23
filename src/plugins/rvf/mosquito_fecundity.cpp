/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "mosquito_fecundity.h"

using namespace UniSim;

namespace rvf {
	
MosquitoFecundity::MosquitoFecundity(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("waterLevel", &waterLevel, 0., this, "desc");
    new Parameter<double>("waterLevelThreshold", &waterLevelThreshold, 20., this, "desc");
    new Parameter<double>("dailyFecundity", &dailyFecundity, 20., this, "desc");
    new Variable<double>("value", &value, this, "desc");
}

void MosquitoFecundity::reset() {
    value = 0;
}

void MosquitoFecundity::update() {
    value = (waterLevel < waterLevelThreshold) ? 0. : dailyFecundity;
}


} //namespace

