/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
/*! \class UniSim::IntegratorMakerPlugIn
    \brief The %IntegratorMakerPlugIn provides an abstract interface for the creation of plug-in integrators.

    Consult the existing integrator plugins for examples how to derive your own plug-in integrators.
*/

#ifndef INTEGRATOR_MAKER_PLUG_IN_H
#define INTEGRATOR_MAKER_PLUG_IN_H

#include <QtPlugin>
#include "identifier.h"

class QString;
class QStringList;

namespace UniSim{

class Integrator;
class ObjectPool;

class IntegratorMakerPlugIn
{
public:
    virtual ~IntegratorMakerPlugIn() { }
	
    //! Returns a list of the integrator types supported by this integrator maker
    virtual Identifiers supportedTypes() const = 0;
	
    //! Returns name of this plugin
    virtual Identifier plugInName() const = 0;

    //! Sets ObjectPool to be used by plug-in
    virtual void useObjectPool(ObjectPool *pool) const = 0;

    //! Create a Integrator object based on its type
	/*!
        \param integratorType type of integrator to create
        \param objectName given to the created integrator
        \param parent of the created integrator
        \return Integrator created or null if integratorType was unknown
	*/
    virtual UniSim::Integrator* create(Identifier integratorType, Identifier objectName, QObject *parent=0) = 0;
};

} //namespace

Q_DECLARE_INTERFACE(UniSim::IntegratorMakerPlugIn, "org.UniSim.UniSimToys.IntegratorMakerPlugIn/0.1")

#endif
