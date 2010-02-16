/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QMap>
#include <usbase/controller.h>
#include <usbase/controller_maker_plug_in.h>
#include <usbase/exception.h>
#include <usbase/file_locations.h>
#include <usbase/object_pool.h>
#include <usbase/utilities.h>
#include "controller_maker.h"

namespace UniSim{

ControllerMaker *ControllerMaker::_me = 0;

ControllerMaker::ControllerMaker() {
    lookupPlugIns<ControllerMakerPlugIn>(id(), &_ControllerMakers);
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
