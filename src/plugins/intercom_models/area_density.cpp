/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <QMessageBox>
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include "area_density.h"
#include "plant.h"

using namespace UniSim;

namespace intercom{

AreaDensity::AreaDensity(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
}

void AreaDensity::initialize() {
    Plant *plant = seekOneAscendant<Plant*>("*");
    plantHeightModel = plant->seekOneDescendant<Model*>("height");
}

void AreaDensity::reset() {
    update();
}

void AreaDensity::update() {
    plantHeight = plantHeightModel->pullVariable<double>("height");
}

} //namespace

