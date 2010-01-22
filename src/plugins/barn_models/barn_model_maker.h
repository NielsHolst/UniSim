#ifndef BARN_MODEL_MAKER_H
#define BARN_MODEL_MAKER_H

#include <usbase/model_maker_plug_in.h>

namespace UniSim {

	class ObjectPool;

}


namespace barn {

class BarnModelMaker : public QObject, public UniSim::ModelMakerPlugIn
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

