/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/utilities.h>
#include "area.h"
#include "layer.h"
#include "community.h"
#include "constants.h"
#include "plant.h"
#include "plant_layers.h"

using namespace UniSim;

namespace intercom{

Layer::Layer(Identifier name, QObject *parent, int layer_)
    : NamedObject(name, parent), layer(layer_)
{
}

void Layer::initialize() {
    calendar = seekOne<Model*>("calendar");

    Community *community = seekOneAscendant<Community*>("*");
    allAreas = community->seekDescendants<Area*>("*");

    PlantLayers *plantLayers = seekOneAscendant<PlantLayers*>("*");
    Plant *plant = plantLayers->plant();
    plantHeight = plant->seekOneChild<Model*>("height");
    plantAreas = plant->seekDescendants<Area*>("*");
}

PhotosyntheticRate Layer::calcPhotosynthesis() {
    if (calendar->pullVariable<double>("sinb") == 0.)
        return PhotosyntheticRate();

    double h = height();
    LightComponents eaa = calcEffectiveAreaAbove(h);

    PhotosyntheticRate result;
    for (int i = 0; i < plantAreas.size(); ++i) {
        PhotosyntheticRate areaResult = plantAreas[i]->calcPhotosynthesis(h, eaa);
        result += areaResult;
        /*
        result += plantAreas[i]->calcPhotosynthesis(eaa);
        result *= leafDensity;
        */
    }

    double plHeight = plantHeight->pullVariable<double>("height");
    result *= plHeight*WGAUSS5[layer];
    cout << " Layer::calcPhotosynthesis() result = " << result.absorption() << " " << result.assimilation() << "\n";

    return result;
}

LightComponents Layer::calcEffectiveAreaAbove(double height) {
    LightComponents eaa;
    for (int i = 0; i < allAreas.size(); ++i)
        eaa += allAreas[i]->calcEffectiveAreaAbove(height);
    return eaa;
}

double Layer::height() const {
    double plHeight = plantHeight->pullVariable<double>("height");
    return plHeight*XGAUSS5[layer];
}

LightComponents Layer::ELAI() {
    LightComponents eaa;
    double h = height();
    for (int i = 0; i < allAreas.size(); ++i)
        eaa += allAreas[i]->calcEffectiveAreaAbove(h);
    return eaa;
}

} //namespace

