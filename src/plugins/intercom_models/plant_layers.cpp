/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/utilities.h>
#include "plant.h"
#include "plant_layers.h"
#include "layer.h"

using namespace UniSim;

namespace intercom{

PlantLayers::PlantLayers(Identifier name, Plant *plant, QObject *parent)
    : NamedObject(name, parent), _plant(plant)
{
    Q_ASSERT(_plant);
}

void PlantLayers::initialize() {
    for (int i =  0; i < 5; ++i) {
        QString name = "layer" + QString::number(i);
        Layer *layer = new Layer(name, this, i);
        layers.append(layer);
        layer->initialize();
    }
}

Plant* PlantLayers::plant() {
    return _plant;
}

} //namespace

