/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QDir>
#include <QMessageBox>
#include <QPluginLoader>
#include <usbase/integrator.h>
#include <usbase/integrator_maker_plug_in.h>
#include <usbase/exception.h>
#include <usbase/file_locations.h>
#include <usbase/object_pool.h>
#include "integrator_maker.h"

namespace UniSim{

Maker *Maker::_me = 0;

Maker::Maker() {
	QDir dir = FileLocations::location(FileLocationInfo::Plugins);

	foreach (QString filename, dir.entryList(QDir::Files)) {
		QPluginLoader loader(dir.absoluteFilePath(filename));
        MakerPlugIn *Maker = qobject_cast<MakerPlugIn*>(loader.instance());
        if (Maker) {
            Maker->useObjectPool(objectPool());
            foreach (Identifier integratorType, Maker->integratorTypes()) {
                Q_ASSERT(_Makers.find(integratorType) == _Makers.end());
                _Makers[integratorType] = Maker;
			}
		}
    }
    if (_Makers.size() == 0) {
        QString msg = "Found no integrator plugins in: " + dir.absolutePath();
        if (!dir.exists()) msg += ".\nThe folder does not exist.";
        throw Exception(msg);
    }
}

UniSim::Integrator* Maker::create(Identifier integratorType, Identifier objectName, QObject *parent) {
    MakerPlugIn *maker = me()->find(integratorType);
	return maker ? maker->create(integratorType, objectName, parent) : 0;
}

bool Maker::canCreate(Identifier integratorType) {
    return me()->find(integratorType);
}

MakerPlugIn* Maker::find(Identifier integratorType) {
    Makers::iterator mm = _Makers.find(integratorType);
    return (mm == _Makers.end()) ? 0 : mm.value();
}

Maker* Maker::findOne(Identifier id) {
    if (!_me)
        _me = objectPool()->find<Maker*>(id);
    return _me;
}

} //namespace
