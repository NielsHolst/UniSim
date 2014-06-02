/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QMessageBox>
#include <usbase/decode_list.h>
#include <usbase/exception.h>
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include <usbase/utilities.h>
#include "crop.h"
#include "rotation.h"

using namespace UniSim;

namespace awe {

Rotation::Rotation(UniSim::Identifier name, QObject *parent)
    : Model(name,parent)
{
    new Parameter<QString>("crops", &cropsString, QString(), this,
                 "Sequence of crops in rotation. For example, @F {(maize wheat wheat)}, "
                 "in which maize is followed by two crops of wheat. The crop names must "
                 "refer to names of @F Crop models that are children of @F {Rotation}.");
    new Variable<double>("lai", &lai, this,
                     "Leaf area index (m @Sup {2}/m @Sup {2}) of the current crop");
    setRecursionPolicy(Component::Update, Component::ChildrenNot);
}



void Rotation::initialize() {
    decodeCrops();
    _calendar = seekOne<Model*>("calendar");
    collectRotation();
    connectCrops();
}

void Rotation::collectRotation() {
   for (int i = 0; i < cropNames.size(); ++i) {
        Model *crop = seekOneChild<Model*>(cropNames[i]);
        crops.append(crop);
    }
 }

void Rotation::connectCrops() {
    QList<Model*> crops = cropModels();
    for (int i = 0; i < crops.size(); ++i)
        connect(crops[i], SIGNAL(event(QObject*, QString)), this, SLOT(handleEvent(QObject*, QString)));
}

void Rotation::reset() {
    state = NotSown;
    currentCropIndex = 0;
    lai = 0;
}

void Rotation::update() {
    currentCrop()->deepUpdate();
    lai = currentCrop()->pullValue<double>("lai");
}

Model* Rotation::currentCrop() {
    return crops[currentCropIndex];
}

void Rotation::decodeCrops() {
    cropNames = decodeSimpleList(cropsString, this);
}

void Rotation::handleEvent(QObject *, QString event) {
    if (event == "sowing") {
        state = Growing;
    }
    else if (event == "harvest") {
        state = Harvested;
        currentCropIndex = (currentCropIndex + 1)%crops.size();
    }
}

QList<Model*> Rotation::cropRotation(){
    return crops;
}

QList<Model*> Rotation::cropModels(){
    return seekChildren<Model*>("*");
}

}
