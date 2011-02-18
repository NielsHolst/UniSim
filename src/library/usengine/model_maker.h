/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef MODEL_MAKER_H
#define MODEL_MAKER_H

#include <QMap>
#include <QObject>
#include <QStringList>
#include <usbase/identifier.h>

class QString;

namespace UniSim{
	
class Model;
class ModelMakerPlugIn;

//! \addtogroup UniSim
//@{
//! %ModelMaker is a singleton class providing a factory that creates models programmatically 
/*!
    %ModelMaker makes model objects without relying on UniSim input. It can create objects of the model types
	provided as plug-ins. 
	\sa ModelMakerPlugIn.
*/
class ModelMaker : public QObject
{
    Q_OBJECT
public:
    ModelMaker();
    static QString id();

	//! Create a Model object based on its type
	/*!
		The valid model types are those supplied by ModelMakerPlugIn's.
		\param modelType type of model to create
		\param objectName given to the created model 
		\param parent of the created model
		\return Model created or null of modelType was unknown
	*/
    static Model* create(Identifier modelType, Identifier objectName, QObject *parent=0);
	
	//! Checks whether a model type is valid
	/*!
		The valid model types are those supplied by ModelMakerPlugIn's.
	*/
    static bool canCreate(Identifier modelType);
	
	//! Provides the selection of model types available
	/*!
		The valid model types are those supplied by ModelMakerPlugIn's.
	*/
    static Identifiers selection();

    //! Provides a list of the available model plugins
    static QList<ModelMakerPlugIn*> plugins();
	
	private:
    static ModelMaker* me();
    static ModelMaker *_me;

    ModelMakerPlugIn* find(Identifier modelType);
	
    typedef QMap<Identifier, ModelMakerPlugIn*> ModelMakers; //! The key to the map is a model
	ModelMakers _modelMakers;
};
//@}
}//namespace

#endif
