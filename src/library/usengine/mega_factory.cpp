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
#include <usbase/identifier.h>
#include <usbase/file_locations.h>
#include <usbase/model.h>
#include <usbase/factory_plug_in.h>
#include <usbase/object_pool.h>
#include <usbase/utilities.h>
#include "mega_factory.h"

namespace UniSim{

MegaFactory *MegaFactory::_me = 0;
int MegaFactory::productCount = 0;

MegaFactory::MegaFactory() {
    bool keepLooking = true;
    do {
        QDir dir = FileLocations::location(FileLocationInfo::Plugins);
        foreach (QString filename, dir.entryList(QDir::Files)) {
            QString filePath = dir.absoluteFilePath(filename);
            QPluginLoader loader(filePath);
            FactoryPlugIn *factory = qobject_cast<FactoryPlugIn*>(loader.instance());
            if (factory) {
//                std::cout << "Loading plugin: " << qPrintable(filename ) << "\n";
                _factories << factory;
                for (Identifier id : factory->inventory()) {
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
        creation = factory->create(className.withoutNamespace(), adjustedId(objectName), parent);
        creation->setProperty("classLabel", className.label());
        break;
    default:
        QString msg = "More than one model of class '%1'. Qualify type with plug-in name";
        throw Exception(msg.arg(className.label()));
    }
    return creation;
}

Identifier MegaFactory::adjustedId(Identifier id) {
    if (id.key().isEmpty())
        return (QString("_noname_") + QString::number(productCount++));
    return id;
}

const QList<FactoryPlugIn*>& MegaFactory::factories() {
    return me()->_factories;
}

MegaFactory* MegaFactory::me() {
    if (!_me)
        _me = objectPool()->find<MegaFactory*>(id());
    return _me;
}

} //namespace
