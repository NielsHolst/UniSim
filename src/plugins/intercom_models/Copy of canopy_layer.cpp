/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
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

    if (calendar->pullVariable<double>("sinb") == 0.)
        return PhotosyntheticRate();

    LightComponents eaa = calcEffectiveAreaAbove( height() );

    PhotosyntheticRate result;
    double h = height();
    for (int i = 0; i < plantAreas.size(); ++i) {
        double leafDensity = plantAreas[i]->atHeight(h);
        PhotosyntheticRate areaResult = plantAreas[i]->calcPhotosynthesis(eaa);
        areaResult *= leafDensity;
        result += areaResult;
        /*
        result += plantAreas[i]->calcPhotosynthesis(eaa);
        result *= leafDensity;
        */
    }

    double plHeight = plantHeight->pullVariable<double>("height");
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

double CanopyLayer::height() const {
    lookup();
    double plHeight = plantHeight->pullVariable<double>("height");
    return plHeight*XGAUSS5[layer];
}

LightComponents CanopyLayer::ELAI() {
    lookup();
    LightComponents eaa;
    double h = height();
    for (int i = 0; i < allAreas.size(); ++i)
        eaa += allAreas[i]->calcEffectiveAreaAbove(h);
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
