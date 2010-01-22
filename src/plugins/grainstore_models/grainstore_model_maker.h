/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef GRAINSTORE_MODEL_MAKER_H
#define GRAINSTORE_MODEL_MAKER_H

#include <usbase/model_maker_plug_in.h>

namespace UniSim {
    class ObjectPool;
}

namespace grainstore{

class GrainstoreModelMaker : public QObject, public UniSim::ModelMakerPlugIn
{
	Q_OBJECT
	Q_INTERFACES(UniSim::ModelMakerPlugIn)
public:
	QList<UniSim::Identifier> modelTypes() const;
	UniSim::Identifier plugInName() const;
    void useObjectPool(UniSim::ObjectPool *pool) const;
	UniSim::Model* create(UniSim::Identifier modelType,
	                      UniSim::Identifier objectName,
	                      QObject *parent=0);
};

} //namespace

#endif

