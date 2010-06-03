/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INTEGRATOR_MAKER_H
#define INTEGRATOR_MAKER_H

#include <QDir>
#include <QMap>
#include <QObject>

#include <QStringList>
#include <usbase/identifier.h>

class QString;

namespace UniSim{
	
class Integrator;
class IntegratorMakerPlugIn;

//! \addtogroup UniSim
//@{
//! %IntegratorMaker is a singleton class providing a factory that creates integrators programmatically 
/*!
    %IntegratorMaker makes integrator objects without relying on UniSim input. It can create objects of the standard
	integrator types plus any integrator types provided as plug-ins. 
	\sa IntegratorMakerPlugIn.
*/
class IntegratorMaker : public QObject
{
    Q_OBJECT
public:
    IntegratorMaker();
    static QString id();

    //! Create a integrator object based on its type
	/*!
		The valid integrator types include the standard ones plus any supplied by IntegratorMakerPlugIn's.
		\param IntegratorType type of integrator to create
		\param objectName given to the created integrator 
		\param parent of the created integrator
		\return Integrator created or null if IntegratorType was unknown
	*/
    static Integrator* create(Identifier integratorType, Identifier objectName, QObject *parent=0);
	
	//! Checks whether a integrator type is valid
	/*!
		The valid integrator types include the standard ones plus any supplied by IntegratorMakerPlugIn's.
	*/
    static bool canCreate(Identifier integratorType);

private:
    static IntegratorMaker* me();
    static IntegratorMaker *_me;

    IntegratorMakerPlugIn* find(Identifier integratorType);

    typedef QMap<Identifier, IntegratorMakerPlugIn*> IntegratorMakers;
    QMap<Identifier, IntegratorMakerPlugIn*> _integratorMakers;
};
//@}
}//namespace

#endif
