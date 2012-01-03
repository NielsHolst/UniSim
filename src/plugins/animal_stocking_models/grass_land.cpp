/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include "grass_land.h"

using namespace UniSim;


namespace AnimalStocking {

GrassLand::GrassLand(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
}

void GrassLand::initialize() {
}

void GrassLand::reset() {
}

void GrassLand::update() {
}

} //namespace

