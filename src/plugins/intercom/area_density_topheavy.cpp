/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <QMessageBox>
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include "area_density_topheavy.h"
#include "plant.h"

using namespace UniSim;

namespace intercom{

AreaDensityTopheavy::AreaDensityTopheavy(UniSim::Identifier name, QObject *parent)
    : AreaDensity(name, parent)
{
}

double AreaDensityTopheavy::at(double height) const {
    return 30.*pow(height,4)/pow(plantHeight,5)*(1. - height/plantHeight);
}

double AreaDensityTopheavy::above(double height) const {
    return -0.164 + 1.172/(1. + exp(8.352*(height/plantHeight - 0.759)));
}

} //namespace

