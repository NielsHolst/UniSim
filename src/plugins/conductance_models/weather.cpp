/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include "weather.h"

using namespace UniSim;


namespace conductance {

Weather::Weather(UniSim::Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Parameter<double>("irradiation", &irradiation, 10., this,
                 "Constant value");
    new PullVariable<double>("irradiation", &irradiation, this,
                     "Global irradiation (MJ/m @Sup {2}/d");
}

void Weather::reset() {
    irradiation = parameter<double>("irradiation");
}

void Weather::update() {
    irradiation = parameter<double>("irradiation");
}

} //namespace

