/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/utilities.h>
#include "insect1.h"

using namespace UniSim;


namespace beehive{

Insect1::Insect1(UniSim::Identifier name, QObject *parent)
	: Model(name, parent) {
}

void Insect1::initialize() {
    setParameter("initEggs", &initEggs, 30.);
    setParameter("sexRatio", &sexRatio, 0.5);

    egg = UniSim::findChild<Model*>("egg", this);
    larva = UniSim::findChild<Model*>("larva", this);
    pupa = UniSim::findChild<Model*>("pupa", this);
    adult = UniSim::findChild<Model*>("adult", this);
}

void Insect1::reset() {
    egg->setInput("inflow", initEggs);
}

void Insect1::update() {
}

} //namespace

