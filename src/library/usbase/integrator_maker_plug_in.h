/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
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

#include <QMap>
#include <QtPlugin>
#include "identifier.h"

class QString;
class QStringList;

namespace UniSim{

class Integrator;

class IntegratorMakerPlugIn
{
public:
    virtual ~IntegratorMakerPlugIn() { }
	
    //! Returns name of this plugin
    virtual Identifier pluginName() const = 0;

    //! Returns description of this plugin
    virtual QString pluginDesc() const = 0;

    //! Returns the model types supported by this model maker and their description
    virtual const QMap<Identifier, QString>& supportedClasses() = 0;

    //! Returns the list of authors for this plugin
    virtual QStringList authors() const = 0;

    //! Create a Integrator object based on its type
	/*!
        \param integratorType type of integrator to create
        \param objectName given to the created integrator
        \param parent of the created integrator
        \return Integrator created or null if integratorType was unknown
	*/
    virtual UniSim::Integrator* create(Identifier integratorType, Identifier objectName, QObject *parent=0) = 0;
protected:
    QMap<Identifier, QString> desc;
};

} //namespace

Q_DECLARE_INTERFACE(UniSim::IntegratorMakerPlugIn, "org.ecolmod.UniSim.IntegratorMakerPlugIn/0.x")

#endif
