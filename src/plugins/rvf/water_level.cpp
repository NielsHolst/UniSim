/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "publish.h"
#include "water_level.h"

using namespace UniSim;

namespace rvf {

PUBLISH(WaterLevel)

WaterLevel::WaterLevel(Identifier name, QObject *parent)
    : Model(name, parent)
{
    Input(double, dailyLoss, 0.2);
    Output(double, rainfall);
        /* Daily average rainfall, either read from records file,
           or calculated as the daily average of minimum and maximum rainfall. */
    Output(double, value);
}
void WaterLevel::initialize() {
//  Must have a child model called "records"
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

