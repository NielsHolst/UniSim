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
#include "plant_layers.h"
#include "time_slice.h"

using namespace UniSim;

namespace intercom{

TimeSlice::TimeSlice(Identifier name, QObject *parent, int slice_)
    : NamedObject(name, parent), slice(slice_)
{
}

void TimeSlice::initialize() {
    calendar = seekOne<Model*>("calendar");
    QList<Plant*> plants = seekMany<Plant*>("*");
    for (int i = 0; i < plants.size(); ++i) {
        QString name = plants[i]->id().label() + "Layers";
        PlantLayers *layers = new PlantLayers(name , this);
        plantLayers.append(layers);
        layers->initialize();
    }
}

PhotosyntheticRate TimeSlice::calcPhotosynthesis() {
    double dayLength = calendar->pullVariable<double>("dayLength");
    double hour = 12. + 0.5*dayLength*XGAUSS3[slice];
    clock()->doTick(hour);

    PhotosyntheticRate result;
    for (int i = 0; i < plants.size(); ++i)
        result += plants[i]->calcPhotosynthesis();

    result *= dayLength*WGAUSS3[slice];
    cout << " TimeSlice::calcPhotosynthesis() result = " << result.absorption() << " " << result.assimilation() << "\n";
    return result;
}

} //namespace

