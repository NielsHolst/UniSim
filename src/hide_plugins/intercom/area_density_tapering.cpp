/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <QMessageBox>
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include "area_density_tapering.h"
#include "plant.h"

using namespace UniSim;

namespace intercom{

AreaDensityTapering::AreaDensityTapering(UniSim::Identifier name, QObject *parent)
    : AreaDensity(name, parent)
{
}

double AreaDensityTapering::at(double height) const {
    return ((-0.2*height/plantHeight + 0.2)/0.1)/plantHeight;
}

double AreaDensityTapering::above(double height) const {
    double x = height/plantHeight;
    return x*x - 2.*x + 1.;
}

} //namespace

