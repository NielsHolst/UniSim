/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "canopy_layer.h"
#include "constants.h"
#include "organ.h"
#include "plant.h"

namespace intercom{

Plant::Plant(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    for (int i = 0; i < 5; ++i)
        canopyLayers.append(new CanopyLayer(i, this));
}

PhotosyntheticRate Plant::calcPhotosynthesis() {
    PhotosyntheticRate result;
    for (int i = 0; i < 5; ++i)
        result += canopyLayers[i]->calcPhotosynthesis();
    return result;
    cout << " Plant::calcPhotosynthesis() result = " << result.absorption() << " " << result.assimilation() << "\n";
}

} //namespace

