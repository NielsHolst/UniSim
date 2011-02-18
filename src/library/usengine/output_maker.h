/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_OUTPUT_MAKER_H
#define UNISIM_OUTPUT_MAKER_H

#include <QMap>
#include <QObject>
#include <usbase/identifier.h>

class QString;

namespace UniSim{
	
class Output;
class OutputMakerPlugIn;

//! %OutputMaker is a singleton class providing a factory that creates output programmatically
/*!
    %OutputMaker makes model objects without relying on UniSim input. It can create objects of the model types
	provided as plug-ins. 
    \sa OutputMakerPlugIn.
*/
class OutputMaker : public QObject
{
    Q_OBJECT
public:
    OutputMaker();
    static QString id();

    //! Create an Outputobject based on its type
	/*!
        The valid output types are those supplied by OutputMakerPlugIn's.
        \param outputType type of model to create
        \param objectName given to the created output
        \param parent of the created output
        \return Output created or null of outputType was unknown
	*/
    static Output* create(Identifier outputType, Identifier objectName, QObject *parent=0);
	
    //! Checks whether an output type is valid
	/*!
        The valid model types are those supplied by OutputMakerPlugIn's.
	*/
    static bool canCreate(Identifier outputType);
	
    //! Provides the selection of output types available
	/*!
        The valid model types are those supplied by OutputMakerPlugIn's.
    */
    static Identifiers selection();
	
	private:
    static OutputMaker* me();
    static OutputMaker *_me;

    OutputMakerPlugIn* find(Identifier outputType);
	
    typedef QMap<Identifier, OutputMakerPlugIn*> OutputMakers;
    OutputMakers _outputMakers;
};

}//namespace

#endif
