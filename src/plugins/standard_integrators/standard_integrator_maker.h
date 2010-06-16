/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef CONTROLLER_MODEL_MAKER_H
#define CONTROLLER_MODEL_MAKER_H

#include <usbase/integrator_maker_plug_in.h>

namespace UniSim{

class ObjectPool;

class StandardIntegratorMaker : public QObject, public IntegratorMakerPlugIn
{
	Q_OBJECT
    Q_INTERFACES(UniSim::IntegratorMakerPlugIn)
public:
    UniSim::Identifier pluginName() const;
    QString pluginDesc() const;
    QStringList authors() const;
    const QMap<UniSim::Identifier, QString>& supportedClasses();

    void useObjectPool(ObjectPool *pool) const;

    Integrator* create(Identifier integratorType, Identifier objectName, QObject *parent=0);
};

} //namespace

#endif


