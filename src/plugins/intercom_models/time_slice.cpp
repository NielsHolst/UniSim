/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <usbase/clock.h>
#include <usbase/model.h>
#include <usbase/utilities.h>
#include "constants.h"
#include "plant.h"
#include "time_slice.h"

using namespace UniSim;

namespace intercom{

TimeSlice::TimeSlice(int slice_, QObject *parent)
    : QObject(parent), calendar(0), slice(slice_)
{
}

PhotosyntheticRate TimeSlice::calcPhotosynthesis() {
    lookup();

    double dayLength = calendar->pullVariable("dayLength");
    double hour = 12. + 0.5*dayLength*XGAUSS3[slice];
    clock()->doTick(hour);

    PhotosyntheticRate result;
    for (int i = 0; i < plants.size(); ++i)
        result += plants[i]->calcPhotosynthesis();

    result *= dayLength*WGAUSS3[slice];
    cout << " TimeSlice::calcPhotosynthesis() result = " << result.absorption() << " " << result.assimilation() << "\n";
    return result;
}

void TimeSlice::lookup() {
    if (!calendar)
        calendar = seekOneDescendant<Model*>("calendar", simulationObject());
    if (plants.size() == 0)
        plants = seekDescendants<Plant*>("*", simulationObject());
}

} //namespace

