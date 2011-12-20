/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <typeinfo>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QtXml/QXmlStreamReader>
#include <usbase/data_grid.h>
#include <usbase/model.h>
#include <usbase/integrator.h>
#include <usbase/exception.h>
#include <usbase/output.h>
#include <usbase/output_parameter.h>
#include <usbase/output_variable.h>
#include <usbase/parameter.h>
#include <usbase/parameter_base.h>
#include <usbase/pull_variable.h>
#include <usbase/pull_variable_base.h>
#include <usbase/utilities.h>
#include "integrator_maker.h"
#include "model_maker.h"
#include "output_maker.h"
#include "simulation.h"
#include "simulation_maker.h"
#include "xml_node.h"
#include "xy_state_variables.h"

namespace UniSim{

namespace {
    inline QList<QObject*> asList(QObject * object) {
        return (QList<QObject*>() << object);
    }
}

SimulationMaker::SimulationMaker()
	: QObject()
{
	reader = new QXmlStreamReader;
}

SimulationMaker::~SimulationMaker()
{
    /*
    QHashIterator<QString, const DataGrid*> i(tables);
    while (i.hasNext()) {
        i.next();
        delete i.value();
    }*/
    delete reader;
}

bool SimulationMaker::nextElementDelim()
{
    QString myTest = elementName();
    QXmlStreamReader::TokenType myType;
    bool unusedElement;
    if (moreToRead()) {
		do {
			reader->readNext();
            myTest = elementName();
            myType = reader->tokenType();
            bool commonElement = reader->isStartElement() && elementNameEquals("common");

            unusedElement = commonElement || !(reader->isStartElement() || reader->isEndElement());
            if (commonElement)
                ignoreElement();
        } while (unusedElement && moreToRead());
	}
	return reader->isStartElement() || reader->isEndElement();
}

bool SimulationMaker::moreToRead() {
    return !reader->hasError() && !reader->isEndDocument();
}

void SimulationMaker::ignoreElement() {
    int levels = 1;
    while (levels>0  && moreToRead()) {
        reader->readNext();
        if (reader->isStartElement())
            ++levels;
        else if (reader->isEndElement())
            --levels;
    }
}

Simulation* SimulationMaker::parse(QString fileName_)
{
    QString simName, simVersion;

    redirectedParameters.clear();
    outputVariableParam.clear();
    outputParameterParam.clear();

    emit beginExpansion();
    fileName = compileToFile(fileName_);
    emit endExpansion();

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QString msg("Cannot open file '%1' for reading.");
        throw Exception(message(msg.arg(fileName)));
    }
	reader->setDevice(&file);

    if (!nextElementDelim())
        throw Exception(message("File is not in valid XML format"));
    if (elementNameNotEquals("simulation"))
        throw Exception(message("Root element must be 'simulation'"));

    simName = attributeValue("name", "anonymous");
    simVersion = attributeValue("version", "1.0");
	
    Simulation *sim = new Simulation(simName, simVersion);
    UniSim::setSimulationObject(sim);
    sim->setFilePath(fileName_);

    nextElementDelim();
    int numIntegrators(0);
    bool noModels(true), noOutputs(true);
    while (!reader->hasError() && reader->isStartElement()) {
        if (elementNameEquals("integrator")) {
            readIntegratorElement(sim);
            ++numIntegrators;
        } else if (elementNameEquals("model")) {
            readModelElement(asList(sim));
            noModels = false;
        } else if (elementNameEquals("output")) {
            readOutputElement(sim);
            noOutputs = false;
		} else {
            QString msg("Unexpected element: '%1'");
            throw Exception(message(msg.arg(elementName())), sim);
		}
	}
	Q_ASSERT(reader->isEndElement());
    if (reader->hasError())
        throw Exception(message(""));
    if (numIntegrators == 0)
        throw Exception(message("Missing 'integrator' element in 'simulation'"));
    if (numIntegrators > 1)
        throw Exception(message("Only one 'integrator' element allowed in 'simulation'"));
    if (noModels)
        throw Exception(message("Missing 'model' element in 'simulation'"));
    if (noOutputs)
        throw Exception(message("Missing 'output' element in 'simulation'"));

    redirectParameters();
    setSequence(sim);
    reader->clear();
    emit beginInitialization();
    sim->initialize(_sequence, this);
    emit endInitialization();

    return sim;
}

QString SimulationMaker::compileToFile(QString filePath) {
    QDir dir = FileLocations::location(FileLocationInfo::Temporary);
    QString fileName = QFileInfo(filePath).baseName();
    fileName += "_compiled.xml";
    QString compiledFilePath = dir.absolutePath() + "/" + fileName;

    XmlNode *original = XmlNode::createFromFile(filePath);
    original->compile(filePath);
    original->writeToFile(compiledFilePath);
    delete original;
    return compiledFilePath;
}

void SimulationMaker::readIntegratorElement(QObject* parent) {
	Q_ASSERT(reader->isStartElement() && parent);
	
    QString type = attributeValue("type", parent);
    QString name = attributeValue("name", "anonymous");

    Integrator *integrator;
    try {
        integrator = IntegratorMaker::create(type, name, parent);
    }
    catch (Exception &ex) {
        throw Exception(message(ex.message()));
    }
	
	_sequence.clear();
	nextElementDelim();
	while (!reader->hasError() && reader->isStartElement()) {
        if (elementNameEquals("sequence")) {
			readSequenceElement(integrator);
        }
        else if (elementNameEquals("parameter")){
            readParameterElement(asList(integrator));
        }
        else if (elementNameEquals("model")){
            readModelElement(asList(integrator));
        }
        else {
            QString msg("Unexpected element: '%1'");
            throw Exception(message(msg.arg(elementName())), parent);
		}
	}	
	Q_ASSERT(reader->isEndElement());
	nextElementDelim();
}	

void SimulationMaker::readSequenceElement(QObject* parent)
{
	Q_ASSERT(reader->isStartElement() && parent);

	nextElementDelim();
	while (!reader->hasError() && reader->isStartElement()) {
        if (elementNameEquals("model")) {
            QString model = attributeValue("name", parent);
            _sequence.append(model);
        }
        else {
            QString msg("Unknown child element of 'sequence' element: '%1'");
            throw Exception(message(msg.arg(elementName())), parent);
		}
		nextElementDelim();
		Q_ASSERT(reader->isEndElement());
		nextElementDelim();
	}	
	Q_ASSERT(reader->isEndElement());

	nextElementDelim();
}

void SimulationMaker::readModelElement(QList<QObject*> parents) {
    Q_ASSERT(!parents.isEmpty());
    Q_ASSERT(reader->isStartElement());

    const DataGrid *table = createTable();
    QList<QObject*> models;
    for (int i = 0; i < parents.size(); ++i) {;
        QObject *parent = parents[i];
        QVector<int> indices = instanceIndices(table, parent);
        models << createModelElement(indices, table, parent);
    }

	nextElementDelim();
    if (!models.isEmpty()) {
        while (!reader->hasError() && reader->isStartElement()) {
            if (elementNameEquals("model"))
                readModelElement(models);
            else if (elementNameEquals("parameter"))
                readParameterElement(models);
            else {
                QString msg("Unexpected element: '%1'");
                throw Exception(message(msg.arg(elementName())));
            }
        }
    }
	Q_ASSERT(reader->isEndElement());
	nextElementDelim();

    delete table;
}

const DataGrid* SimulationMaker::createTable() {
    QString fileName = attributeValue("table", "");
    if (fileName.isEmpty())
        return 0;

    QString filePath = simulation()->inputFilePath(fileName);
    if (tables.contains(filePath))
        return tables.value(filePath);

    DataGrid *table = new DataGrid(filePath);
    if (table->numKeys() == 0)
        throw Exception("Table must hold at least one column marked as key with '*'");
    tables[filePath] = table;
    return table;
}

QVector<int> SimulationMaker::instanceIndices(const DataGrid *table, QObject *parent) {
    QString instancesStr = attributeValue("instances", "");
    bool hasInstances = !instancesStr.isEmpty();
    if (table && hasInstances)
        throw Exception("Set either attribute 'instances' or 'fileName', not both");

    int numIndices;
    QVector<int> indices;
    enum {All, One, Lookup} method;
    if (table)
        if (table->numKeys() == 1) {
            method = All;
            numIndices = table->rowNames().size();
        }
        else
            method = Lookup;
    else if (hasInstances) {
        method = All;
        bool ok(true);
        numIndices = instancesStr.toInt(&ok);
        if (!ok || numIndices <= 0)
            throw Exception(message("Attribute 'instances'' must a number larger than zero"));
    }
    else
        method = One;

    switch (method) {
    case One:
        indices << -1;  // use object name for the one instance
        break;
    case All:
        for (int i = 0; i < numIndices; ++i) {
            indices << i;
        }
        break;
    case Lookup:
        DataGrid::KeySubset keys;
        int excludeKey = columnIndexOfModelType(table);
        for (int i = 0; i < table->numKeys(); ++i) {
            if (i==excludeKey) continue;
            Identifier id = Identifier( table->columnNames().value(i) );
            Model *model = dynamic_cast<Model*>(parent);
            Q_ASSERT(model);
            keys[i] = lookupKeyValue(model, id);
        }
        indices = QVector<int>::fromList( table->rowIndices(keys) );
    }
    return indices;
}

QList<QObject*> SimulationMaker::createModelElement(
            const QVector<int> &instanceIndices,
            const DataGrid *table,
            QObject *parent)
{
    QString modelType = attributeValue("type", "anonymous");
    QString hide = attributeValue("hide", "");
    QList<QObject*> models;
    Models instances;
    int numInstances = instanceIndices.size();
    try {
        for (int i = 0; i < numInstances; ++i) {
            int index = instanceIndices.value(i);
            Model *model = ModelMaker::create(modelType, instanceName(index, table), parent);
            models << model;
            instances << model;

            if (table)
                updateColumnParameters(model, *table);

            if (!hide.isEmpty()) {
                bool isHidden = UniSim::stringToValue<bool>(hide);
                model->setHide(isHidden);
            }
        }
        for (int i = 0; i < numInstances; ++i) {
            Model *prev = (i==0) ? 0 : instances[i-1];
            Model *next = (i<numInstances-1) ? instances[i+1] : 0;
            instances[i]->setInstances(prev, next);

        }
    }
    catch (Exception &ex) {
        throw Exception(message(ex.message()), parent);
    }
    return models;
}

QString SimulationMaker::instanceName(int instanceIndex, const DataGrid *table) {
    if (table)
        return table->cell(instanceIndex, columnIndexOfModelType(table));
    QString objectName = attributeValue("name", "anonymous");
    return (instanceIndex==-1) ? objectName : QString("%1(%2)").arg(objectName).arg(instanceIndex+1);
}

int SimulationMaker::columnIndexOfModelType(const DataGrid *table) {
    QString modelType = attributeValue("type", "");
    int col = table->columnNames().indexOf(modelType);
    if (col == -1) {
        QString msg("Table needs a column heading: '%1'");
        throw Exception(msg.arg("*"+modelType));
    }
    return col;
}

namespace {
    Identifier classId(Model *model) {
        QVariant label = model->property("classLabel");
        Q_ASSERT(label.isValid());
        return Identifier(label.toString());
    }
}

QString SimulationMaker::lookupKeyValue(Model *model, Identifier keyId) {
    Identifier id = classId(model);
    if (id == keyId)
        return model->id().label();

    Model *ascendant = model->peekParent<Model*>("*");
    while (ascendant) {
        Identifier id = classId(ascendant);
        if (id == keyId)
            return ascendant->id().label();
        QList<ParameterBase*> parameter = ascendant->seekChildren<ParameterBase*>(keyId.label());
        if (parameter.size() == 1)
            return parameter.value(0)->toString();
        Q_ASSERT(parameter.isEmpty());
        ascendant = ascendant->peekParent<Model*>("*");
    }
    QString msg("Unable to find value for key value '%1' in table");
    throw Exception(msg.arg(keyId.label()), model);
}

void SimulationMaker::updateColumnParameters(Model *model, const DataGrid &table) {
    QStringList keys;
    int numKeys = table.numKeys();
    for (int i = 0; i < numKeys; ++i) {
        Identifier id = Identifier( table.columnNames().value(i) );
        keys << lookupKeyValue(model, id);
    }
    for (int j = numKeys; j < table.columnNames().size(); ++j) {
        QString columnName = table.columnNames().value(j);
        ParameterBase *parameter = seekOneChild<ParameterBase*>(columnName, model);
        QString value = table.cell(keys, columnName);
        parameter->setValueFromString(value);
    }
}

void SimulationMaker::readParameterElement(QList<QObject*> parents)
{
    Q_ASSERT(!parents.isEmpty());
    Q_ASSERT(reader->isStartElement());
    for (int i = 0; i < parents.size(); ++i) {
        setParameterElement(parents[i]);
    }
	nextElementDelim();
	Q_ASSERT(reader->isEndElement());
	nextElementDelim();
}

void SimulationMaker::setParameterElement(QObject *parent) {
    QString name = attributeValue("name", "");
    QString value = attributeValue("value", "");
    QString variableName = attributeValue("variable", "");
    QString table = attributeValue("table", "");

    int numEmpty = value.isEmpty() + variableName.isEmpty() + table.isEmpty();
    if (numEmpty != 2) {
        QString msg("Parameter '%1' must have either a 'value', a 'variable' or a 'table' attribute");
        throw Exception(message(msg.arg(name)), parent);
    }

    if (name.isEmpty() && table.isEmpty()) {
        QString msg("Parameter '%1' must have either a 'name' or a 'table' attribute");
        throw Exception(message(msg.arg(name)), parent);
    }

    if (!table.isEmpty()) {
        const DataGrid *table = createTable();
        Model *model = dynamic_cast<Model*>(parent);
        if (!model)
            throw Exception (message("Parent of a 'table' parameter must be a model"), parent);
        updateColumnParameters(model, *table);
    }
    else {
        ParameterBase *parameter = seekOneChild<ParameterBase*>(name, parent);
        if (!value.isEmpty())
            parameter->setValueFromString(value.trimmed());
        else
            redirectedParameters.append(RedirectedParameter(parameter, variableName));
    }
}

void SimulationMaker::readOutputElement(QObject* parent)
{
	Q_ASSERT(reader->isStartElement() && parent);
	
    QString objectName = attributeValue("name", "anonymous");
    QString type = attributeValue("type", parent);
    QString summary = attributeValue("summary", "");

    Output *output;
    try {
        output = OutputMaker::create(type, objectName, parent);
        if (!summary.isEmpty()) {
            bool isSummary = UniSim::stringToValue<bool>(summary);
            output->setIsSummary(isSummary);
        }
    }
    catch (Exception &ex) {
        throw Exception(message(ex.message()));
    }

	nextElementDelim();
	while (!reader->hasError() && reader->isStartElement()) {
        if (elementNameEquals("parameter")) {
            readParameterElement(asList(output));
        }
        else if (elementNameEquals("read-parameter")) {
            readOutputSubElement(&outputParameterParam, output);
        }
        else if (elementNameEquals("variable")) {
            readOutputSubElement(&outputVariableParam, output);
        }
        else {
            throw Exception(message("Unexpected element: '" + elementName() + "'"), parent);
        }
    }
	Q_ASSERT(reader->isEndElement());
	nextElementDelim();
}	

void SimulationMaker::readOutputSubElement(QList<OutputParam> *parameters, QObject* parent)
{

    Q_ASSERT(reader->isStartElement() && parent);

    OutputParam param;
    param.attributes["label"] = attributeValue("label", parent);
    param.attributes["value"] = attributeValue("value", parent);
    param.attributes["axis"] = attributeValue("axis", parent);
    param.attributes["summary"] = attributeValue("summary", "");
    param.attributes["type"] = attributeValue("type", "");
    param.parent = parent;

    parameters->append(param);

    nextElementDelim();
    Q_ASSERT(reader->isEndElement());
    nextElementDelim();
}

void SimulationMaker::setupOutputVariableElements()
{
    for (int i = 0; i < outputVariableParam.size(); ++i) {
        OutputParam param = outputVariableParam[i];
        QString value = param.attributes.value("value");
        QList<PullVariableBase*> variables = seekMany<QObject*, PullVariableBase*>(value);
        for (int j = 0; j < variables.size(); ++j) {
            new OutputVariable(param.attributes, variables[j], param.parent);
        }
    }
}

void SimulationMaker::setupOutputParameterElements()
{
    for (int i = 0; i < outputParameterParam.size(); ++i) {
        OutputParam param = outputParameterParam[i];
        QString value = param.attributes.value("value");
        QList<ParameterBase*> parameters = seekMany<QObject*, ParameterBase*>(value);
        for (int i = 0; i < parameters.size(); ++i)
            new OutputParameter(param.attributes, parameters[i], param.parent);
    }
}

void SimulationMaker::splitOutputDataValue(QString value, QString *datasetName, QString *columnName) {
    bool ok = false;
    QStringList items = value.trimmed().split("[");
    if (items.size() == 2 && items[1].endsWith("]")) {
        ok = true;
        *datasetName = items[0];
        *columnName = items[1].left( items[1].size() - 1 );
    }
    if (!ok) {
        QString msg("Value badly formatted: '%1");
        throw Exception(message(msg.arg(value)));
    }
}

bool SimulationMaker::elementNameEquals(QString s) const {
    return reader->name().compare(s, Qt::CaseInsensitive) == 0;
}

bool SimulationMaker::elementNameNotEquals(QString s) const {
    return reader->name().compare(s, Qt::CaseInsensitive) != 0;
}

QString SimulationMaker::elementName() const {
    return reader->name().toString();
}

QString SimulationMaker::attributeValue(QString name, QString defaultValue) const {
    QString result = reader->attributes().value(name).toString().trimmed();
    return result.isEmpty() ? defaultValue : result;
}

QString SimulationMaker::attributeValue(QString name, QObject *parent) const {
    QString result = attributeValue(name, "");
    if (result.isEmpty()) {
        QString msg("Missing attribute: '%1'");
        throw Exception(message(msg.arg(name)), parent);
    }
    return result;
}

QString SimulationMaker::message(QString text) const {
    QString s;
    QTextStream str(&s);
    str   << "Error reading UniSim file: "  <<  fileName << "\n"
          << "In line: " << reader->lineNumber() << "\n"
          << "In position: " << reader->columnNumber() << "\n"
          << (text.isEmpty() ? QString() : "Error message: " + text) << "\n"
          << "XML-reader reported: " << reader->errorString() << "\n"
          << "Last token read: " << reader->tokenString();
    return s;
}

namespace {
    template <class T>
    bool couple(ParameterBase *parameter, PullVariableBase *variable) {
        Parameter<T>* parameterT = dynamic_cast<Parameter<T>*>(parameter);
        PullVariable<T> *variableT = dynamic_cast<PullVariable<T>*>(variable);
        bool matchedT = parameterT && variableT;
        if (matchedT) {
            T *redirectTo = const_cast<T*>(variableT->valuePtr());
            parameterT->redirectValuePtr(redirectTo);
            Component *sender = seekNearestAscendant<Component*>("*", parameter);
            Component *receiver = seekNearestAscendant<Component*>("*", variable);
            QObject::connect(sender, SIGNAL(pullVariableChanged(PullVariableBase*,ParameterBase*)),
                             receiver, SLOT(acceptPullVariableChanged(PullVariableBase*,ParameterBase*)));
        }
        return matchedT;
    }
}

void SimulationMaker::redirectParameters() {
    for (int i = 0; i < redirectedParameters.size(); ++i) {
        ParameterBase *parameter = redirectedParameters[i].first;
        QString variableName = redirectedParameters[i].second;
        PullVariableBase *variable = seekOne<QObject*, PullVariableBase*>(variableName);
        bool coupled =
            couple<bool>(parameter, variable) ||
            couple<int>(parameter, variable) ||
            couple<long>(parameter, variable) ||
            couple<unsigned>(parameter, variable) ||
            couple<float>(parameter, variable) ||
            couple<double>(parameter, variable) ||
            couple<QDate>(parameter, variable) ||
            couple<QString>(parameter, variable);
        if (!coupled) {
            QString msg("The type of variable '%1' does not match that of the parameter");
            throw Exception(message(msg.arg(variableName)), parameter);
        }
    }
}

void SimulationMaker::setSequence(Simulation *sim) {
    if (!_sequence.isEmpty()) return;

    Models models = seekChildren<Model*>("*", sim);
    for (int i = 0; i < models.size(); ++i) {
        _sequence << models.value(i)->id();
    }
}

} //namespace
