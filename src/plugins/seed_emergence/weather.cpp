/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include "weather.h"

using namespace UniSim;


namespace seed_emergence {

Weather::Weather(UniSim::Identifier name, QObject *parent)
    : Model(name, parent)
{
    Output(double, Tavg); // Soil temperature ({@Degree}C)
    Output(double, swp); // Soil water potential in MPa
}

void Weather::initialize() {
    records = seekOneChild<Model*>("records");
}

void Weather::reset() {
    update();
}

void Weather::update() {
    Tavg = records->pullValue<double>("Tavg");
    swp = records->pullValue<double>("SWPkPA")/1000.;
}

} //namespace

