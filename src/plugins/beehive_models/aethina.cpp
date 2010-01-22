/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "aethina.h"

using namespace UniSim;


namespace beehive{

Aethina::Aethina(UniSim::Identifier name, QObject *parent)
	: Model(name, parent) {
	setState("N", &density);
}

void Aethina::initialize() {
	setParameter("Ninit", &Ninit, 1.);
	setParameter("K", &K, 1000.);
	setParameter("r", &r, 1.2);
}

void Aethina::reset() {
	density = Ninit;
}

void Aethina::update() {
	density += (K<=0) ? 0 : density*r*(K-density)/K;
}

} //namespace

