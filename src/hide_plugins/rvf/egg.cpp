/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

#include "egg.h"


using namespace UniSim;

namespace rvf {

Egg::Egg(Identifier name, QObject *parent)
     : Model(name, parent)
{
    new Variable<double>("totalEggsInflow", &totalEggsInflow, this, "desc");
}

void Egg::initialize() {
    Model *parent = seekParent<Model*>("*");
    inactiveEgg = parent->seekOneChild<Model*>("inactiveEgg");
    adult = parent->seekOneChild<Model*>("adult");
}

void Egg::reset() {
    totalEggsInflow = 0.;
}

void Egg::update() {
    double numberOfEggsActivated = inactiveEgg->pullValue<double>("eggsActivated");
    double numberOfEggsLaidInwater = adult->pullValue<double>("eggsLaidInWater");
    totalEggsInflow = numberOfEggsActivated + numberOfEggsLaidInwater;
}

} //namespace
