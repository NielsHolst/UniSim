/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
/*! \class UniSim::OutputMakerPlugIn
    \brief The %OutputMakerPlugIn provides an abstract interface for the creation of plug-in controllers.

    Consult the existing controller plugins for examples how to derive your own plug-in controllers.
*/

#ifndef OUTPUT_MAKER_PLUG_IN_H
#define OUTPUT_MAKER_PLUG_IN_H

#include <QtPlugin>
#include "identifier.h"

namespace UniSim{

class Output;
class ObjectPool;

class OutputMakerPlugIn
{
public:
	virtual ~OutputMakerPlugIn() { }	
	
	//! Returns a list of the output types supported by this output maker
    virtual Identifiers supportedTypes() const = 0;
	
    //! Returns name of this plugin
    virtual Identifier plugInName() const = 0;

    //! Sets ObjectPool to be used by plug-in
    virtual void useObjectPool(ObjectPool *pool) const = 0;

    //! Create an Output object based on its type
	/*!
		\param outputType type of output to create
		\param objectName given to the created output 
		\param parent of the created output
        \return Output created, or null if controllerType was unknown
	*/
    virtual UniSim::Output* create(Identifier outputType, Identifier objectName, QObject *parent=0) = 0;
};

} //namespace

Q_DECLARE_INTERFACE(UniSim::OutputMakerPlugIn, "org.UniSim.UniSimToys.OutputMakerPlugIn/0.1")

#endif
