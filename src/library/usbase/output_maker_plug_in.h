/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
/*! \class UniSim::OutputMakerPlugIn
    \brief The %OutputMakerPlugIn provides an abstract interface for the creation of plug-in integrators.

    Consult the existing integrator plugins for examples how to derive your own plug-in integrators.
*/

#ifndef OUTPUT_MAKER_PLUG_IN_H
#define OUTPUT_MAKER_PLUG_IN_H

#include <QMap>
#include <QtPlugin>
#include "identifier.h"

namespace UniSim{

class Output;

class OutputMakerPlugIn
{
public:
	virtual ~OutputMakerPlugIn() { }	
	
    //! Returns name of this plugin
    virtual Identifier pluginName() const = 0;

    //! Returns description of this plugin
    virtual QString pluginDesc() const = 0;

    //! Returns the model types supported by this model maker and their description
    virtual const QMap<Identifier, QString>& supportedClasses() = 0;

    //! Returns the list of authors for this plugin
    virtual QStringList authors() const = 0;

    //! Create an Output object based on its type
	/*!
		\param outputType type of output to create
		\param objectName given to the created output 
		\param parent of the created output
        \return Output created, or null if integratorType was unknown
	*/
    virtual UniSim::Output* create(Identifier outputType, Identifier objectName, QObject *parent=0) = 0;
protected:
    QMap<Identifier, QString> desc;
};

} //namespace

Q_DECLARE_INTERFACE(UniSim::OutputMakerPlugIn, "org.ecolmod.UniSim.OutputMakerPlugIn/0.x")

#endif
