/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_INTEGRATOR_MAKER_H
#define UNISIM_INTEGRATOR_MAKER_H

#include <usbase/integrator_maker_plug_in.h>

namespace UniSim{

class ObjectPool;

class UniSimIntegratorMaker : public QObject, public IntegratorMakerPlugIn
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


