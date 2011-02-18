/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_SIMULATION_MAKER
#define UNISIM_SIMULATION_MAKER
#include <QList>
#include <QObject>
#include <QPair>
#include <QStringList>
#include <usbase/identifier.h>

class QXmlStreamReader;

namespace UniSim{

class ParameterBase;
class Parameters;
class Simulation;

//! \addtogroup UniSim
//@{
//! The %SimulationMaker class provides a factory to create simulations from UniSim files
class SimulationMaker : public QObject
{
	Q_OBJECT
	
public:
	//! Constructor
	SimulationMaker();
	//! Destructor
	~SimulationMaker();
    // Create Simulation object from UniSim file
	/*!
		Parsing happens in two steps:
	
        (1) Expansion, in which a temporary UniSim file is constructed by following all references, in the
        original UniSim file, to other UniSim files. The resulting UniSim file has the contents of the
		original plus the contents of all files referred to (recursively). XmlExpander is used to expand the 
        original UniSim file.
		
		(2) Creation, in which the Simulation object and all its children are created by interpreting
        the expanded UniSim file. ModelMaker is used to create the models specified in UniSim.
			
        \param fileName name of UniSim file
		\return New simulation object
	*/	
    Simulation* parse(QString fileName);

    void setupOutputVariableElements();
    void setupOutputDataElements();
    void setupOutputParameterElements();

signals: 
    //! Signals when the parser begins expanding the original UniSim file
	void beginExpansion();
    //! Signals when the parser ends expanding the original UniSim file
	void endExpansion();
    //! Signals when the parser begins initializing components in simulation
    void beginInitialization();
    //! Signals when the parser ends initializing components in simulation
    void endInitialization();

private:
    // data
    QXmlStreamReader *reader;
    QString fileName;
    Identifiers _sequence;

    typedef QPair<ParameterBase*, QString> RedirectedParameter;
    QList<RedirectedParameter> redirectedParameters;

    struct OutputParam {
        QString axis, label, value, summary;
        QObject *parent;
    };

    QList<OutputParam> outputVariableParam, outputDataParam, outputParameterParam;

    // methods
    bool nextElementDelim();
	
	bool readIntegratorElement(QObject* parent);
	void readSequenceElement(QObject *parent);

	bool readModelElement(QObject *parent);
    void readDatasetElement(QObject* parent);
    void readParameterElement(QObject *parent);
    void readOutputParameterElement(QObject *parent);

	bool readOutputElement(QObject *parent);
    void readOutputVariableElement(QObject* parent);
    void readOutputDataElement(QObject* parent);

    void splitOutputDataValue(QString value, QString *datasetName, QString *columnName);
    bool elementNameEquals(QString s) const;
    bool elementNameNotEquals(QString s) const;
    QString elementName() const;
    QString attributeValue(QString name, QString defaultValue) const;
    QString attributeValue(QString name, QObject *parent) const;

    QString message(QString text) const;
    void redirectParameters();
};

//@}
}//namespace

#endif
