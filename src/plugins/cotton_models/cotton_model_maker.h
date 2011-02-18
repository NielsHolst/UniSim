/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef COTTON_MODEL_MAKER_H
#define COTTON_MODEL_MAKER_H

#include <usbase/model_maker_plug_in.h>

namespace UniSim {
    class ObjectPool;
}

namespace cotton{

class CottonModelMaker : public QObject, public UniSim::ModelMakerPlugIn
{
	Q_OBJECT
	Q_INTERFACES(UniSim::ModelMakerPlugIn)
public:
    UniSim::Identifier pluginName() const;
    QString pluginDesc() const;
    QStringList authors() const;
    const QMap<UniSim::Identifier, QString>& supportedClasses();

    void useObjectPool(UniSim::ObjectPool *pool) const;

    UniSim::Model* create(UniSim::Identifier modelType,
	                      UniSim::Identifier objectName,
	                      QObject *parent=0);
};

} //namespace

#endif

