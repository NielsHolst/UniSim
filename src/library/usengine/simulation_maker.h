/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
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
#include <usbase/identifier.h>

class QXmlStreamReader;

namespace UniSim{

class DataGrid;
class Model;
class ParameterBase;
class Parameters;
class Simulation;

class SimulationMaker : public QObject
{
	Q_OBJECT
	
public:
	SimulationMaker();
	~SimulationMaker();
    Simulation* parse(QString fileName);

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
    QStack<QPair<QString, Model*> > keyStack;
    QHash<QString, const DataGrid*> tables;

    typedef QPair<ParameterBase*, QString> RedirectedParameter;
    QList<RedirectedParameter> redirectedParameters;

    struct TraceParam : public Attributes {
        QObject *parent;
    };

    QList<TraceParam> traceVariableParam, traceParameterParam;

    // methods
    QString compileToFile(QString filePath);

    bool nextElementDelim();
    bool moreToRead();
    void ignoreElement();

    void readIntegratorElement(QObject* parent);
    void readSequenceElement(QObject *parent);

    void readModelElement(QList<QObject*> parents);
    QList<QObject*> createModelElement(const QVector<int> &instanceIndices, const DataGrid *table, QObject *parent);
    const DataGrid* createTable();
    QVector<int> instanceIndices(const DataGrid *table, QObject *parent);
    QString instanceName(int instanceIndex, const DataGrid *table);
    int columnIndexOfModelType(const DataGrid *table);
    void updateColumnParameters(Model *model, const DataGrid &table);
    QString lookupKeyValue(Model *model, Identifier keyId);

    void readParameterElement(QList<QObject*> parents);
    void setParameterElement(QObject *parent);

    void readOutputElement(QObject *parent);
    void readOutputSubElement(QList<TraceParam> *param, QObject* parent);
    template <class T, class U>
        void createTracesKindOf(const QList<TraceParam> &traceParam);

    bool elementNameEquals(QString s) const;
    bool elementNameNotEquals(QString s) const;
    QString elementName() const;
    QString attributeValue(QString name, QString defaultValue) const;
    QString attributeValue(QString name, QObject *parent) const;

    QString message(QString text) const;
    void amendComponents();
    void redirectParameters();
    void createTraces();
};

//@}
}//namespace

#endif
