/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QDir>
#include <QPluginLoader>
#include <QSettings>
#include <usbase/exception.h>
#include <usbase/file_locations.h>
#include <usbase/model.h>
#include <usbase/model_maker_plug_in.h>
#include <usbase/object_pool.h>
#include "model_maker.h"

namespace UniSim{

ModelMaker *ModelMaker::_me = 0;

ModelMaker::ModelMaker()
{
	QDir dir = FileLocations::location(FileLocations::Plugins);
	
	foreach (QString filename, dir.entryList(QDir::Files))  {
		QPluginLoader loader(dir.absoluteFilePath(filename));
		ModelMakerPlugIn *modelMaker = qobject_cast<ModelMakerPlugIn*>(loader.instance());
		if (modelMaker) {
            modelMaker->useObjectPool(objectPool());
            foreach (Identifier modelType, modelMaker->modelTypes()) {
				_modelMakers[modelType] = modelMaker;
                Identifier nsModelType = modelMaker->plugInName().label()+"::"+modelType.label();
                _modelMakers[nsModelType] = modelMaker;
			}
		}
	}
    if (_modelMakers.size() == 0) {
        QString msg = "Found no model plugins in: " + dir.absolutePath();
        if (!dir.exists()) msg += ".\nThe folder does not exist.";
        throw Exception(msg);
    }
}

QString ModelMaker::id() {
    return "ModelMaker";
}

UniSim::Model* ModelMaker::create(Identifier modelType, Identifier objectName, QObject *parent)
{
    ModelMakerPlugIn *maker;
    switch (me()->_modelMakers.count(modelType)) {
        case 0:
            throw Exception("No model of type: " + modelType.key());
        case 1:
            maker = me()->find(modelType);
            return maker->create(modelType.withoutNamespace(), objectName, parent);
        default:
            throw Exception("More than one model of type: " + modelType.key()+". Qualify type with plug-in name.");
    }

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
