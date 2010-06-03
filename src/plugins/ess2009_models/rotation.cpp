/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QMessageBox>
#include <usbase/exception.h>
#include <usbase/pull_variable.h>
#include <usbase/utilities.h>
#include "crop.h"
#include "rotation.h"

using namespace UniSim;

namespace ess2009 {

Rotation::Rotation(UniSim::Identifier name, QObject *parent)
    : Model(name,parent)
{
    setRecursionPolicy(Component::Update, Component::ChildrenNot);
    new PullVariable("lai", &_lai, this);
}



void Rotation::initialize() {
    setParameter("crops", &_cropsString, QString());
    decodeCrops();
    _calendar = seekOne<Model*>("calendar");
    collectRotation();
    connectCrops();
}

void Rotation::collectRotation() {
   for (int i = 0; i < _cropNames.size(); ++i) {
        Model *crop = seekOne<Model*>(_cropNames[i]);
        _crops.append(crop);
    }
 }

void Rotation::connectCrops() {
    QList<Model*> crops = cropModels();
    for (int i = 0; i < crops.size(); ++i)
        connect(crops[i], SIGNAL(event(QObject*, QString)), this, SLOT(handleEvent(QObject*, QString)));
}

void Rotation::reset() {
    _state = NotSown;
    _currentCropIndex = 0;
    _lai = 0;
}

void Rotation::update() {
    currentCrop()->deepUpdate();
    _lai = currentCrop()->pullVariable("lai");
}

Model* Rotation::currentCrop() {
    return _crops[_currentCropIndex];
}

void Rotation::decodeCrops() {
    _cropNames = decodeSimpleList(_cropsString, "Crop rotation");
}

void Rotation::handleEvent(QObject *sender, QString event) {
    if (event == "sowing") {
        _state = Growing;
    }
    else if (event == "harvest") {
        _state = Harvested;
        _currentCropIndex = (_currentCropIndex + 1)%_crops.size();
    }
}

QList<Model*> Rotation::cropRotation(){
    return _crops;
}

QList<Model*> Rotation::cropModels(){
    return seekChildren<Model*>("*");
}

}
