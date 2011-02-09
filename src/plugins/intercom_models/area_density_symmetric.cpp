/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <QMessageBox>
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include "area_density_symmetric.h"
#include "plant.h"

using namespace UniSim;

namespace intercom{

AreaDensitySymmetric::AreaDensitySymmetric(UniSim::Identifier name, QObject *parent)
    : AreaDensity(name, parent)
{
}

double AreaDensitySymmetric::at(double height) const {
    return 6./pow(plantHeight,3)*height*(plantHeight - height);
}

double AreaDensitySymmetric::above(double height) const {
    return 1. - 1./pow(plantHeight,3)*height*height*(3.*plantHeight - 2.*height);
}

} //namespace

