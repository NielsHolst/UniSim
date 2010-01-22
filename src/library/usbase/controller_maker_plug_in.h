/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
/*! \class UniSim::ControllerMakerPlugIn
    \brief The %ControllerMakerPlugIn provides an abstract interface for the creation of plug-in controllers.

    Consult the existing controller plugins for examples how to derive your own plug-in controllers.
*/

#ifndef CONTROLLER_MAKER_PLUG_IN_H
#define CONTROLLER_MAKER_PLUG_IN_H

#include <QtPlugin>
#include "identifier.h"

class QString;
class QStringList;

namespace UniSim{

class Controller;
class ObjectPool;

class ControllerMakerPlugIn
{
public:
	virtual ~ControllerMakerPlugIn() { }	
	
	//! Returns a list of the controller types supported by this controller maker
    virtual Identifiers controllerTypes() const = 0;
	
    //! Returns name of this plugin
    virtual Identifier plugInName() const = 0;

    //! Sets ObjectPool to be used by plug-in
    virtual void useObjectPool(ObjectPool *pool) const = 0;

    //! Create a Controller object based on its type
	/*!
		\param controllerType type of controller to create
		\param objectName given to the created controller 
		\param parent of the created controller
        \return Controller created or null if controllerType was unknown
	*/
    virtual UniSim::Controller* create(Identifier controllerType, Identifier objectName, QObject *parent=0) = 0;
};

} //namespace

Q_DECLARE_INTERFACE(UniSim::ControllerMakerPlugIn, "org.UniSim.UniSimToys.ControllerMakerPlugIn/0.1")

#endif
