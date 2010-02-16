/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef CONTROLLER_MODEL_MAKER_H
#define CONTROLLER_MODEL_MAKER_H

#include <usbase/controller_maker_plug_in.h>

namespace UniSim{

class ObjectPool;

class StandardControllerMaker : public QObject, public ControllerMakerPlugIn
{
	Q_OBJECT
    Q_INTERFACES(UniSim::ControllerMakerPlugIn)
public:
    QList<Identifier> supportedTypes() const;
    UniSim::Identifier plugInName() const;
    void useObjectPool(ObjectPool *pool) const;
    Controller* create(Identifier controllerType, Identifier objectName, QObject *parent=0);
};

} //namespace

#endif


