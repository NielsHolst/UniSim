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

ControllerMaker *ControllerMaker::_me = 0;

ControllerMaker::ControllerMaker() {
	QDir dir = FileLocations::location(FileLocations::Plugins);

	foreach (QString filename, dir.entryList(QDir::Files)) {
		QPluginLoader loader(dir.absoluteFilePath(filename));
        ControllerMakerPlugIn *ControllerMaker = qobject_cast<ControllerMakerPlugIn*>(loader.instance());
        if (ControllerMaker) {
            ControllerMaker->useObjectPool(objectPool());
            foreach (Identifier controllerType, ControllerMaker->controllerTypes()) {
				Q_ASSERT(_ControllerMakers.find(controllerType) == _ControllerMakers.end());
				_ControllerMakers[controllerType] = ControllerMaker;
			}
		}
    }
    if (_ControllerMakers.size() == 0) {
        QString msg = "Found no controller plugins in: " + dir.absolutePath();
        if (!dir.exists()) msg += ".\nThe folder does not exist.";
        throw Exception(msg);
    }
}

QString ControllerMaker::id() {
    return "ControllerMaker";
}

UniSim::Controller* ControllerMaker::create(Identifier controllerType, Identifier objectName, QObject *parent) {
    ControllerMakerPlugIn *maker = me()->find(controllerType);
	return maker ? maker->create(controllerType, objectName, parent) : 0;
}

bool ControllerMaker::canCreate(Identifier controllerType) {
    return me()->find(controllerType);
}

ControllerMakerPlugIn* ControllerMaker::find(Identifier controllerType) {
    ControllerMakers::iterator mm = _ControllerMakers.find(controllerType);
	return (mm == _ControllerMakers.end()) ? 0 : mm.value();
}

ControllerMaker* ControllerMaker::me() {
    if (!_me)
        _me = objectPool()->find<ControllerMaker*>(id());
    return _me;
}

} //namespace
