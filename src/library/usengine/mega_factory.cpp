/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
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
#include <usbase/factory_plug_in.h>
#include <usbase/object_pool.h>
#include <usbase/utilities.h>
#include "mega_factory.h"

namespace UniSim{

MegaFactory *MegaFactory::_me = 0;

MegaFactory::MegaFactory() {
    bool keepLooking = true;
    do {
        QDir dir = FileLocations::location(FileLocationInfo::Plugins);
        foreach (QString filename, dir.entryList(QDir::Files)) {
            QString filePath = dir.absoluteFilePath(filename);
            QPluginLoader loader(filePath);
            FactoryPlugIn *factory = qobject_cast<FactoryPlugIn*>(loader.instance());
            if (factory) {
                _factories << factory;
                factory->defineProducts();
                FactoryPlugIn::Products products_ = products(factory);
                QList<Identifier> classNames = products_.keys();
                for (int i=0; i<classNames.size(); ++i) {
                    Identifier id = classNames[i];
                    productIndex[id] = factory;
                    Identifier idWithNamespace = factory->id().label() + "::" + id.label();
                    productIndex[idWithNamespace] = factory;

                }
            }
        }

        if (productIndex.size() > 0) {
            keepLooking = false;
        }
        else {
            QString msg = "Found no valid plugins in: " + dir.absolutePath();
            if (!dir.exists())
                msg += ".\nThe folder does not exist.";
            keepLooking = FileLocations::lookup(FileLocationInfo::Plugins, msg);
        }
    } while (keepLooking);
}

QString MegaFactory::id() {
    return "MegaFactory";
}

QObject* MegaFactory::createObject(Identifier className, Identifier objectName, QObject *parent)
{
    FactoryPlugIn *factory;
    QObject *creation;
    switch (me()->productIndex.count(className)) {
    case 0:
        throw Exception("No model of class: " + className.key());
    case 1:
        factory = me()->productIndex[className];
        creation = UniSim::create(factory, className.withoutNamespace(), objectName, parent);
        creation->setProperty("classLabel", className.label());
        break;
    default:
        throw Exception("More than one model of class: " + className.key()+". Qualify type with plug-in name.");
    }
    return creation;
}

const QList<FactoryPlugIn*>& MegaFactory::factories() {
    return me()->_factories;
}


//bool MegaFactory::canCreate(Identifier className)
//{
//    return me()->creation.count(className) > 0;
//}

//Identifiers MegaFactory::selection()
//{
//    Identifiers selection;
//    for (ModelMakers::iterator ma = me()->_modelMakers.begin();
//                               ma != me()->_modelMakers.end(); ++ma)
//		selection << ma.key();
//	return selection;
//}

//QList<ModelMakerPlugIn*> MegaFactory::plugins() {
//    QList<ModelMakerPlugIn*> plugins, redundant = me()->_modelMakers.values();
//    for (int i = 0; i < redundant.size(); ++i) {
//        ModelMakerPlugIn *next = redundant[i];
//        if (!plugins.contains(next))
//            plugins.append(next);
//    }
//    return plugins;
//}

//ModelMakerPlugIn* MegaFactory::find(Identifier modelType)
//{
//	ModelMakers::iterator ma = _modelMakers.find(modelType);
//	return (ma == _modelMakers.end()) ? 0 : ma.value();
//}

MegaFactory* MegaFactory::me() {
    if (!_me)
        _me = objectPool()->find<MegaFactory*>(id());
    return _me;
}

} //namespace
