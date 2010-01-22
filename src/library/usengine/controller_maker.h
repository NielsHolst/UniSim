/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef CONTROLLER_MAKER_H
#define CONTROLLER_MAKER_H

#include <QMap>
#include <QObject>

#include <QStringList>
#include <usbase/identifier.h>

class QString;

namespace UniSim{
	
class Controller;
class ControllerMakerPlugIn;

//! \addtogroup UniSim
//@{
//! %ControllerMaker is a singleton class providing a factory that creates controllers programmatically 
/*!
    %ControllerMaker makes controller objects without relying on UniSim input. It can create objects of the standard
	controller types plus any controller types provided as plug-ins. 
	\sa ControllerMakerPlugIn.
*/
class ControllerMaker : public QObject
{
    Q_OBJECT
public:
    ControllerMaker();
    static QString id();

    //! Create a controller object based on its type
	/*!
		The valid controller types include the standard ones plus any supplied by ControllerMakerPlugIn's.
		\param ControllerType type of controller to create
		\param objectName given to the created controller 
		\param parent of the created controller
		\return Controller created or null if ControllerType was unknown
	*/
    static Controller* create(Identifier controllerType, Identifier objectName, QObject *parent=0);
	
	//! Checks whether a controller type is valid
	/*!
		The valid controller types include the standard ones plus any supplied by ControllerMakerPlugIn's.
	*/
    static bool canCreate(Identifier controllerType);

	private:
    static ControllerMaker* me();
    static ControllerMaker *_me;

    ControllerMakerPlugIn* find(Identifier controllerType);

    typedef QMap<Identifier, ControllerMakerPlugIn*> ControllerMakers;
    QMap<Identifier, ControllerMakerPlugIn*> _ControllerMakers;
};
//@}
}//namespace

#endif
