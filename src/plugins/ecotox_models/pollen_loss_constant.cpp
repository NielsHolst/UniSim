/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include "pollen_loss_constant.h"

using namespace UniSim;


namespace ecotox {

PollenLossConstant::PollenLossConstant(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("halfLife", &halfLife, 14., this,
                          "Half-life of pollen (days). Set this to zero if pollen is not lost");
    new PullVariable<double>("value", &value, this,
                             "Pollen survival rate (per day)");
}

void PollenLossConstant::reset() {
    update();
}

void PollenLossConstant::update() {
    double survival = (halfLife > 0) ? pow(0.5, 1./halfLife) : 1.;
    value = 1. - survival;
}


} //namespace

