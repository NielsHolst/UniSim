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

    typedef QPair<ParameterBase*, QString> RedirectedParameter;
    QList<RedirectedParameter> redirectedParameters;

    struct TraceParam : public Attributes {
        QObject *parent;
    };
    QList<TraceParam> traceVariableParam;

    // methods
    void clearTables();
    QString compileToFile(QString filePath);

    bool nextElementDelim();
    bool moreToRead();
    void ignoreElement();

    void readIntegratorElement(QObject* parent);
    void readSequenceElement(QObject *parent);

    void readModelElement(QList<QObject*> parents);
    QList<QObject*> createModelElement(InstanceIndex *table, QObject *parent);
    UniSim::InstanceIndex *createIndexTable();

    void readParameterElement(QList<QObject*> parents);
    void setParameterElement(QObject *parent);
    UniSim::ParameterIndex *createParameterTable(QString fileName);

    void readOutputElement(QObject *parent);
    void readOutputTableElement(QObject* parent);
    void readOutputSubElement(QObject *parent);

    bool elementNameEquals(QString s) const;
    bool elementNameNotEquals(QString s) const;
    QString elementName() const;
    QString attributeValue(QString name, QString defaultValue) const;
    QString attributeValue(QString name, QObject *parent) const;
    QString attributeValue(QStringList synonyms, QString defaultValue) const;
    QString attributeValue(QStringList synonyms, QObject *parent) const;

    QString message(QString text) const;
    void redirectParameters();
    void createTraces();
};

//@}
}//namespace

#endif
