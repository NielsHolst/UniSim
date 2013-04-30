/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_SIMULATION_MAKER
#define UNISIM_SIMULATION_MAKER
#include <QHash>
#include <QList>
#include <QMap>
#include <QObject>
#include <QPair>
#include <QStack>
#include <QStringList>
#include <usbase/attributes.h>
#include <usbase/file_location_info.h>
#include <usbase/identifier.h>

class QXmlStreamReader;

namespace UniSim{

class InstanceIndex;
class Model;
class NamedObject;
class ParameterBase;
class ParameterIndex;
class Parameters;
class Simulation;

class SimulationMaker : public QObject
{
	Q_OBJECT
	
public:
	SimulationMaker();
	~SimulationMaker();
    Simulation* parse(QString filePath);
    Simulation* parse(FileLocationInfo::FileType fileType, QString subFolder, QString fileName);

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
    QHash<QString, InstanceIndex*> instanceTables;
    QHash<QString, ParameterIndex*> parameterTables;

    struct TraceParam : public Attributes {
        NamedObject *parent;
    };
    QList<TraceParam> traceVariableParam;

    // methods
    void clearTables();
    QString compileToFile(QString filePath);

    bool nextElementDelim();
    bool moreToRead();
    void ignoreElement();

    void readIntegratorElement(NamedObject *parent);
    void readSequenceElement(NamedObject *parent);

    void readModelElement(QList<NamedObject*> parents);
    QList<NamedObject*> createModelElement(InstanceIndex *table, NamedObject *parent);
    UniSim::InstanceIndex *createIndexTable();

    void readParameterElement(QList<NamedObject*> parents);
    void setParameterElement(NamedObject *parent);
    UniSim::ParameterIndex *createParameterTable(QString fileName);

    void readOutputElement(NamedObject *parent);
    void readOutputTableElement(NamedObject* parent);
    void readOutputSubElement(NamedObject *parent);

    bool elementNameEquals(QString s) const;
    bool elementNameNotEquals(QString s) const;
    QString elementName() const;
    QString attributeValue(QString name, QString defaultValue) const;
    QString attributeValue(QString name, NamedObject *parent) const;
    QString attributeValue(QStringList synonyms, QString defaultValue) const;
    QString attributeValue(QStringList synonyms, NamedObject *parent) const;

    QString message(QString text) const;
//    void redirectParameters();
    void createTraces();
};

//@}
}//namespace

#endif
