/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/pull_variable.h>
#include <usbase/utilities.h>
#include "area.h"
#include "canopy_layer.h"
#include "community.h"
#include "constants.h"
#include "plant.h"

using namespace UniSim;

namespace intercom{

CanopyLayer::CanopyLayer(int layer_, QObject *parent)
    : QObject(parent), layer(layer_), plantHeight(0), calendar(0)
{
}

PhotosyntheticRate CanopyLayer::calcPhotosynthesis() {
    lookup();

    if (calendar->pullVariable("sinb") == 0.)
        return PhotosyntheticRate();

    double plHeight = plantHeight->pullVariable("height");
    double height = plHeight*XGAUSS5[layer];

    LightComponents eaa = calcEffectiveAreaAbove(height);

    PhotosyntheticRate result;
    for (int i = 0; i < plantAreas.size(); ++i) {
        double leafDensity = plantAreas[i]->atHeight(height);
        PhotosyntheticRate areaResult = plantAreas[i]->calcPhotosynthesis(eaa);
        areaResult *= leafDensity;
        result += areaResult;
        /*
        result += plantAreas[i]->calcPhotosynthesis(eaa);
        result *= leafDensity;
        */
    }

    result *= plHeight*WGAUSS5[layer];
    cout << " CanopyLayer::calcPhotosynthesis() result = " << result.absorption() << " " << result.assimilation() << "\n";

    return result;
}

LightComponents CanopyLayer::calcEffectiveAreaAbove(double height) {
    lookup();
    LightComponents eaa;
    for (int i = 0; i < allAreas.size(); ++i)
        eaa += allAreas[i]->calcEffectiveAreaAbove(height);
    return eaa;
}

void CanopyLayer::lookup() {
    if (!calendar)
        calendar = seekOne<Model*>("calendar");

    if (!plantHeight)
        plantHeight = seekOneChild<Model*>("height", parent());

    if (allAreas.size() == 0) {
        Community *community = seekOneAscendant<Community*>("*", this);
        allAreas = seekDescendants<Area*>("*", community);
    }

    if (plantAreas.size() == 0)
        plantAreas = seekDescendants<Area*>("*", parent());
}


} //namespace

