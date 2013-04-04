/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include "pollen_loss_constant.h"

using namespace UniSim;


namespace ecotox {

PollenLossConstant::PollenLossConstant(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("rate", &rate, 0.01, this,
                          "Fraction of pollen lost per day. Set this to zero if pollen is not lost");
    new Variable<double>("value", &value, this,
                             "Fraction of pollen lost per day");
}

void PollenLossConstant::reset() {
    update();
}

void PollenLossConstant::update() {
    value = rate;
}


} //namespace

