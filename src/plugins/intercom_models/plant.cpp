/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/pull_variable.h>
#include "light_components.h"
#include "constants.h"
#include "organ.h"
#include "plant.h"

using namespace UniSim;

namespace intercom{

Plant::Plant(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    new PullVariable<double>("heights", heights, this,
                             "Array of heights (0..4)");
    new PullVariable<double>("ELAIdiffuse", ELAIdiffuse, this,
                             "Array of effective LAI for diffuse light (0..4)");
    new PullVariable<double>("ELAIdirectdirect", ELAIdirectdirect, this,
                             "Array of effective LAI for direct-direct light (0..4)");
    new PullVariable<double>("ELAIdirecttotal", ELAIdirecttotal, this,
                             "Array of effective LAI for direct total light (0..4)");
}

void Plant::reset() {
    for (int i = 0; i < 5; ++i) {
        heights[i] = 0;
        ELAIdiffuse[i] = 0;
        ELAIdirectdirect[i] = 0;
        ELAIdirecttotal[i] = 0;
    }
}

void Plant::update() {
    /*
    for (int i = 0; i < 5; ++i) {
        heights[i] = canopyLayers[i]->height();
        LightComponents elai = canopyLayers[i]->ELAI();
        ELAIdiffuse[i] = elai[Diffuse];
        ELAIdirectdirect[i] = elai[DirectDirect];
        ELAIdirecttotal[i] = elai[DirectTotal];
    }*/
}


PhotosyntheticRate Plant::calcPhotosynthesis() {
    PhotosyntheticRate result;
    /*for (int i = 0; i < 5; ++i)
        result += canopyLayers[i]->calcPhotosynthesis();*/
    return result;
    cout << " Plant::calcPhotosynthesis() result = " << result.absorption() << " " << result.assimilation() << "\n";
}

} //namespace

