/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
/*! \class UniSim::ModelMakerPlugIn
    \brief The %ModelMakerPlugIn provides an abstract interface for the creation of plug-in models.

    Consult the existing model plugins for examples how to derive your own plug-in models.
*/

#ifndef MODEL_MAKER_PLUG_IN_H
#define MODEL_MAKER_PLUG_IN_H

#include <QMap>
#include <QtPlugin>
#include "identifier.h"

namespace UniSim{

class Model;
	
class ModelMakerPlugIn
{
public:
    virtual ~ModelMakerPlugIn() { }
	
    //! Returns name of this plugin
    virtual Identifier pluginName() const = 0;

    //! Returns description of this plugin
    virtual QString pluginDesc() const = 0;

    //! Returns the model types supported by this model maker and their description
    virtual const QMap<Identifier, QString>& supportedClasses() = 0;

    //! Returns the list of authors for this plugin
    virtual QStringList authors() const = 0;

    //! Create a Model object based on its type
	/*!
		\param modelType type of model to create
		\param objectName given to the created model 
		\param parent of the created model
		\return Model created or null of modelType was unknown
	*/
    virtual UniSim::Model* create(Identifier modelType, Identifier objectName, QObject *parent=0) = 0;
protected:
    QMap<Identifier, QString> desc;
};


} //namespace

Q_DECLARE_INTERFACE(UniSim::ModelMakerPlugIn, "org.ecolmod.UniSim.ModelMakerPlugIn/0.x")

#endif
