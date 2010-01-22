/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QDir>
#include <QMessageBox>
#include <QPluginLoader>
#include <usbase/controller.h>
#include <usbase/controller_maker_plug_in.h>
#include <usbase/exception.h>
#include <usbase/file_locations.h>
#include <usbase/object_pool.h>
#include "controller_maker.h"

namespace UniSim{

Maker *Maker::_me = 0;

Maker::Maker() {
	QDir dir = FileLocations::location(FileLocations::Plugins);

	foreach (QString filename, dir.entryList(QDir::Files)) {
		QPluginLoader loader(dir.absoluteFilePath(filename));
        MakerPlugIn *Maker = qobject_cast<MakerPlugIn*>(loader.instance());
        if (Maker) {
            Maker->useObjectPool(objectPool());
            foreach (Identifier controllerType, Maker->controllerTypes()) {
                Q_ASSERT(_Makers.find(controllerType) == _Makers.end());
                _Makers[controllerType] = Maker;
			}
		}
    }
    if (_Makers.size() == 0) {
        QString msg = "Found no controller plugins in: " + dir.absolutePath();
        if (!dir.exists()) msg += ".\nThe folder does not exist.";
        throw Exception(msg);
    }
}

UniSim::Controller* Maker::create(Identifier controllerType, Identifier objectName, QObject *parent) {
    MakerPlugIn *maker = me()->find(controllerType);
	return maker ? maker->create(controllerType, objectName, parent) : 0;
}

bool Maker::canCreate(Identifier controllerType) {
    return me()->find(controllerType);
}

MakerPlugIn* Maker::find(Identifier controllerType) {
    Makers::iterator mm = _Makers.find(controllerType);
    return (mm == _Makers.end()) ? 0 : mm.value();
}

Maker* Maker::findOne(Identifier id) {
    if (!_me)
        _me = objectPool()->find<Maker*>(id);
    return _me;
}

} //namespace
