/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <typeinfo>
#include <QDir>
#include <QPluginLoader>
#include <QSettings>
#include <usbase/exception.h>
#include <usbase/file_locations.h>
#include <usbase/model.h>
#include <usbase/model_maker_plug_in.h>
#include <usbase/object_pool.h>
#include <usbase/utilities.h>
#include "model_maker.h"

namespace UniSim{

ModelMaker *ModelMaker::_me = 0;

ModelMaker::ModelMaker()
{
    lookupPlugIns<ModelMakerPlugIn>(id(), &_modelMakers);
}

QString ModelMaker::id() {
    return "ModelMaker";
}

UniSim::Model* ModelMaker::create(Identifier modelType, Identifier objectName, QObject *parent)
{
    ModelMakerPlugIn *maker;
    Model *model;
    switch (me()->_modelMakers.count(modelType)) {
    case 0:
        throw Exception("No model of type: " + modelType.key());
    case 1:
        maker = me()->find(modelType);
        model = maker->create(modelType.withoutNamespace(), objectName, parent);
        model->setProperty("classLabel", modelType.label());
        break;
    default:
        throw Exception("More than one model of type: " + modelType.key()+". Qualify type with plug-in name.");
    }
    return model;
}

Models ModelMaker::create(Identifier modelType, Identifier objectName, QObjectList &parents)
{
    Models models;
    for (int i = 0; i < models.size(); ++i)
        models << create(modelType, objectName, parents[i]);
    return models;
}

bool ModelMaker::canCreate(Identifier modelType)
{
    return me()->_modelMakers.count(modelType) > 0;
}

Identifiers ModelMaker::selection()
{
    Identifiers selection;
    for (ModelMakers::iterator ma = me()->_modelMakers.begin();
                               ma != me()->_modelMakers.end(); ++ma)
		selection << ma.key();
	return selection;
}

QList<ModelMakerPlugIn*> ModelMaker::plugins() {
    QList<ModelMakerPlugIn*> plugins, redundant = me()->_modelMakers.values();
    for (int i = 0; i < redundant.size(); ++i) {
        ModelMakerPlugIn *next = redundant[i];
        if (!plugins.contains(next))
            plugins.append(next);
    }
    return plugins;
}

ModelMakerPlugIn* ModelMaker::find(Identifier modelType)
{
	ModelMakers::iterator ma = _modelMakers.find(modelType);
	return (ma == _modelMakers.end()) ? 0 : ma.value();
}

ModelMaker* ModelMaker::me() {
    if (!_me)
        _me = objectPool()->find<ModelMaker*>(id());
    return _me;
}

} //namespace
