/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "constants.h"
#include "organ.h"
#include "plant.h"

namespace intercom{

Plant::Plant(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    setRecursionPolicy(Component::Update, Component::ChildrenNot);
    setState("absorption", &absorption.organ);
    setState("assimilation", &assimilation.organ);
}

void Plant::initialize()
{
    childrenToUpdate.clear();
    childrenToUpdate.append( findChild<Model*>("photoThermalTime") );
    childrenToUpdate.append( findChild<Model*>("leafStemRatio") );
    childrenToUpdate.append( height = findChild<Model*>("height") );
    organs = seekChildren<Organ*>("*");
}

void Plant::reset()
{
}

void Plant::prepareUpdate() {
    for (int i = 0; i < childrenToUpdate.size(); ++i)
        childrenToUpdate[i]->update();
    plantHeight = height->state("height");
    absorption.reset();
    assimilation.reset();
}

void Plant::update()
{
    for (int i = 0; i < organs.size(); ++i)
        organs[i]->update();

    for (int i = 0; i < organs.size(); ++i) {
        absorption.organ += organs[i]->state("absorption");
        assimilation.organ += organs[i]->state("assimilation");
    }
}

LightComponents Plant::weightedAreaAboveLayer(int layerStep) {
    double layerHeight = Wgauss1[layerStep]*plantHeight;
    LightComponents wa;
    for (int i = 0; i < organs.size(); ++i)
        wa.accumulate( organs[i]->weightedAreaAboveLayer(layerHeight) );
    return wa;
}

void Plant::accumulateLightUseByLayer(int layerStep) {
    double layerHeight = Wgauss1[layerStep]*plantHeight;

    absorption.layer += absorption.organ*layerHeight;
    absorption.organ = 0.;

    assimilation.layer += assimilation.organ*layerHeight;
    assimilation.organ = 0.;
}

void Plant::accumulateLightUseByTime(int timeStep) {
    absorption.time += absorption.layer*Wgauss2[timeStep];
    absorption.layer = 0.;

    assimilation.time += assimilation.layer*Wgauss2[timeStep];
    assimilation.layer = 0.;
}

} //namespace

