/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "water_level.h"

using namespace UniSim;

namespace rvf {
WaterLevel::WaterLevel(Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Parameter<double>("dailyLoss", &dailyLoss, 0.2, this, "desc");
    new Variable<double>("rainfall", &rainfall, this, "Daily average rainfall, either read from records file, " "or calculated as the daily average of minimum and maximum rainfall. " "Must have a child model called @F {records}.");
    new Variable<double>("value", &value, this, "desc");
}
void WaterLevel::initialize() {
    Model *records = seekOneChild<Model*>("records");
    Variable<double> *pullRainfall = records->peekOneChild<Variable<double>*>("rainfall");

    rainfallIsPresent = pullRainfall;
    if (rainfallIsPresent) {
        ptrRainfall = pullRainfall->valuePtr();
    }
}
void WaterLevel::reset() {
    value = rainfall;
}
void WaterLevel::update() {
    rainfall = rainfallIsPresent ? *ptrRainfall : 0.;
    value += rainfall - dailyLoss;
    if (value < 0.)
        value = 0.;
}
} //namespace

