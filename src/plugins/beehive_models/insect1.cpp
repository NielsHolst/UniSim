/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/utilities.h>
#include "insect1.h"

using namespace UniSim;


namespace beehive{

Insect1::Insect1(UniSim::Identifier name, QObject *parent)
	: Model(name, parent) 
{
    new Parameter<double>("initEggs", &initEggs, 30., this, "Initial number of insect eggs");
}

void Insect1::initialize() {
    egg = seekOneChild<Model*>("egg");
    larva = seekOneChild<Model*>("larva");
    pupa = seekOneChild<Model*>("pupa");
    adult = seekOneChild<Model*>("adult");
}

void Insect1::reset() {
    egg->pushVariable("inflow", initEggs);
}

void Insect1::update() {
}

} //namespace

