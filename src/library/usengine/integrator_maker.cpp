/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QMap>
#include <usbase/integrator.h>
#include <usbase/integrator_maker_plug_in.h>
#include <usbase/exception.h>
#include <usbase/file_locations.h>
#include <usbase/object_pool.h>
#include <usbase/utilities.h>
#include "integrator_maker.h"

namespace UniSim{

IntegratorMaker *IntegratorMaker::_me = 0;

IntegratorMaker::IntegratorMaker() {
    lookupPlugIns<IntegratorMakerPlugIn>(id(), &_IntegratorMakers);
}

QString IntegratorMaker::id() {
    return "IntegratorMaker";
}

UniSim::Integrator* IntegratorMaker::create(Identifier integratorType, Identifier objectName, QObject *parent) {
    IntegratorMakerPlugIn *maker = me()->find(integratorType);
	return maker ? maker->create(integratorType, objectName, parent) : 0;
}

bool IntegratorMaker::canCreate(Identifier integratorType) {
    return me()->find(integratorType);
}

IntegratorMakerPlugIn* IntegratorMaker::find(Identifier integratorType) {
    IntegratorMakers::iterator mm = _IntegratorMakers.find(integratorType);
	return (mm == _IntegratorMakers.end()) ? 0 : mm.value();
}

IntegratorMaker* IntegratorMaker::me() {
    if (!_me)
        _me = objectPool()->find<IntegratorMaker*>(id());
    return _me;
}

} //namespace
