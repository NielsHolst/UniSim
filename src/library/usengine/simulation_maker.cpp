/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <typeinfo>
#include <QFile>
#include <QHashIterator>
#include <QMessageBox>
#include <QTextStream>
#include <QtXml/QXmlStreamReader>
#include <usbase/model.h>
#include <usbase/integrator.h>
#include <usbase/exception.h>
#include <usbase/output.h>
#include <usbase/trace.h>
#include <usbase/parameter.h>
#include <usbase/parameter_base.h>
#include <usbase/pull_variable.h>
#include <usbase/pull_variable_base.h>
#include <usbase/utilities.h>
#include "instance_index_from_condensed_table.h"
#include "instance_index_from_table.h"
#include "instance_index_numbered.h"
#include "instance_index_one.h"
#include "integrator_maker.h"
#include "model_maker.h"
#include "output_maker.h"
#include "parameter_index_from_condensed_table.h"
#include "parameter_index_from_table.h"
#include "simulation.h"
#include "simulation_maker.h"
#include "xml_node.h"

namespace UniSim{

namespace {
    inline QList<QObject*> asList(QObject * object) {
        return (QList<QObject*>() << object);
    }

    template <class T> void amend() {
        QList<T> components = seekChildren<T>("*", simulation());
        QListIterator<T>  i(components);
        while (i.hasNext()) {
            i.next()->deepAmend();
        }
    }
}

SimulationMaker::SimulationMaker()
	: QObject()
{
    reader = new QXmlStreamReader;
    instanceTables.clear();
    parameterTables.clear();
}

SimulationMaker::~SimulationMaker()
{
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
    QString simName;

    redirectedParameters.clear();
    traceVariableParam.clear();
    traceParameterParam.clear();
    clearTables();

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
	
    Simulation *sim = new Simulation(simName);
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

    amend<Integrator*>();
    amend<Model*>();
    redirectParameters();
    createTraces();
    amend<Output*>();

    reader->clear();
    clearTables();
    return sim;
}

void SimulationMaker::clearTables() {
    qDeleteAll(instanceTables);
    qDeleteAll(parameterTables);
    instanceTables.clear();
    parameterTables.clear();
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
            // deprecated: sequence.append(model);
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

    QString modelType = attributeValue("type", "anonymous");
    QString parameterName = attributeValue("parameter", "");
    QString context = !parameterName.isEmpty() ? parameterName : modelType;

    InstanceIndex *table = createIndexTable();
    QList<QObject*> models;
    for (int i = 0; i < parents.size(); ++i) {;
        QObject *parent = parents[i];
        table->reset(context, dynamic_cast<Model*>(parent));
        models << createModelElement(table, parent);
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
}

InstanceIndex* SimulationMaker::createIndexTable() {
    QString objectName = attributeValue("name", "");
    QString table = attributeValue("table", "");
    QString crosstab = attributeValue("crosstab", "");
    QString instancesStr = attributeValue("instances", "");
    QString parameterName = attributeValue("parameter", "");
    bool hasTable = !table.isEmpty();
    bool hasCrosstab = !crosstab.isEmpty();
    bool hasParameterName = !parameterName.isEmpty();
    bool hasInstances = !instancesStr.isEmpty();

    if (int(hasTable) + int(hasCrosstab) + int(hasInstances) > 1)
        throw Exception("Set only one of these: 'instances', 'table' and 'crosstab'");
    if (hasParameterName && !hasCrosstab) {
        QString msg("If you set the 'parameter' attribute (to '%1') you must also set the 'crosstab' attribute");
        throw Exception(message(msg.arg(parameterName)));
    }
    if (hasCrosstab && !hasParameterName) {
        QString msg("If you set the 'crosstab' attribute (to '%1') you must also set the 'parameter' attribute");
        throw Exception(message(msg.arg(crosstab)));
    }

    int numInstances(0);
    if (hasInstances) {
        bool ok(true);
        numInstances = instancesStr.toInt(&ok);
        if (!ok || numInstances <= 0)
            throw Exception(message("Attribute 'instances'' must a number larger than zero"));
    }

    if (hasInstances)
        return new InstanceIndexNumbered(objectName, numInstances);
    else if (!(hasTable || hasCrosstab))
        return new InstanceIndexOne(objectName);

    QString fileName = hasTable ? table : crosstab;
    QString filePath = simulation()->inputFilePath(fileName);
    if (instanceTables.contains(filePath))
        return instanceTables.value(filePath);

    if (hasParameterName)
        return instanceTables[filePath] = new InstanceIndexFromCondensedTable(filePath);
    else
        return instanceTables[filePath] = new InstanceIndexFromTable(filePath);
}

QList<QObject*> SimulationMaker::createModelElement(InstanceIndex *table, QObject *parent) {
    QString modelType = attributeValue("type", "anonymous");
    QString hide = attributeValue("hide", "");
    QList<QObject*> models;
    Models instances;

    try {
        while (table->hasNext()) {
            InstanceIndex::Record rec = table->next();
            Model *model = ModelMaker::create(modelType, rec.modelName, parent);
            models << model;
            instances << model;
            int numParam = rec.paramNameValue.size();
            for (int i = 0;i < numParam; ++i) {
                QString name = rec.paramNameValue.keys().at(i);
                QString value = rec.paramNameValue.values().at(i);
                ParameterBase *parameter = seekOneChild<ParameterBase*>(name, model);
                parameter->setValueFromString(value);
            }
            if (!hide.isEmpty()) {
                bool isHidden = UniSim::stringToValue<bool>(hide);
                model->setHide(isHidden);
            }
        }
    }
    catch (Exception &ex) {
        throw Exception(message(ex.message()), parent);
    }
    int numInstances = instances.size();
    for (int i = 0; i < numInstances; ++i) {
        Model *prev = (i==0) ? 0 : instances[i-1];
        Model *next = (i<numInstances-1) ? instances[i+1] : 0;
        instances[i]->setInstances(prev, next);
    }
    return models;
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
    QString crosstab = attributeValue("crosstab", "");

    bool hasName = !name.isEmpty();
    bool hasValue = !value.isEmpty();
    bool hasVariable = !variableName.isEmpty();
    bool hasTable = !table.isEmpty();
    bool hasCrosstab = !crosstab.isEmpty();

    if (hasCrosstab && !hasName) {
        QString msg("If you set the 'crosstab' attribute (to '%1') you must also set the 'name' attribute");
        throw Exception(message(msg.arg(crosstab)), parent);
    }
    if (hasTable && hasName) {
        QString msg("If you set the 'table' attribute (to '%1') you cannnot also set the 'name' (set to %2)attribute. Maybe you meant 'crosstab'?");
        throw Exception(message(msg.arg(crosstab).arg(name)), parent);
    }

    if (hasTable || hasCrosstab) {
        if (hasValue) {
            QString msg("Parameter in table '%1' cannot also gave a 'value'attribute");
            throw Exception(message(msg.arg(fileName)), parent);
        }
        if (hasVariable) {
            QString msg("Parameter in table '%1' cannot also gave a 'variable' attribute");
            throw Exception(message(msg.arg(fileName)), parent);
        }
        QString fileName = hasTable ? table : crosstab;
        ParameterIndex *table = createParameterTable(fileName);
        table->reset(dynamic_cast<Model*>(parent));
        while (table->hasNext()) {
            ParameterIndex::Result theNext = table->next();
            ParameterBase *parameter = seekOneChild<ParameterBase*>(theNext.first, parent);
            parameter->setValueFromString(theNext.second);
        }
    }
    else {
        if (!hasName) {
            QString msg("Parameter must have a name");
            throw Exception(message(msg), parent);
        }
        ParameterBase *parameter = seekOneChild<ParameterBase*>(name, parent);
        if (hasValue)
            parameter->setValueFromString(value.trimmed());
        else if (hasVariable)
            redirectedParameters.append(RedirectedParameter(parameter, variableName));
        else {
            QString msg("Parameter '%1' must have either a 'value' or 'variable' attribute");
            throw Exception(message(msg.arg(name)), parent);
        }
    }
}

ParameterIndex *SimulationMaker::createParameterTable(QString fileName) {
    QString name = attributeValue("name", "");
    bool hasName = !name.isEmpty();
    QString filePath = simulation()->inputFilePath(fileName);
    if (parameterTables.contains(filePath))
        return parameterTables.value(filePath);
    if (hasName)
        return parameterTables[filePath] = new ParameterIndexFromCondensedTable(filePath, name);
    else
        return parameterTables[filePath] = new ParameterIndexFromTable(filePath);
}

void SimulationMaker::readOutputElement(QObject* parent)
{
	Q_ASSERT(reader->isStartElement() && parent);
	
    QString objectName = attributeValue("name", "anonymous");
    QString type = attributeValue("type", parent);

    Output *output;
    try {
        output = OutputMaker::create(type, objectName, parent);
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
            readOutputSubElement(&traceParameterParam, output);
        }
        else if (elementNameEquals("variable")) {
            readOutputSubElement(&traceVariableParam, output);
        }
        else {
            throw Exception(message("Unexpected element: '" + elementName() + "'"), parent);
        }
    }
	Q_ASSERT(reader->isEndElement());
	nextElementDelim();
}	

void SimulationMaker::readOutputSubElement(QList<TraceParam> *parameters, QObject* parent)
{
    Q_ASSERT(reader->isStartElement() && parent);

    TraceParam param;
    param.setAttribute( "label", attributeValue("label", parent) );
    param.setAttribute( "value", attributeValue("value", parent) );
    param.setAttribute( "axis", attributeValue("axis", "y") );
    param.setAttribute( "summary", attributeValue("summary", "") );
    param.setAttribute( "type", attributeValue("type", "") );
    param.parent = parent;
    parameters->append(param);

    nextElementDelim();
    Q_ASSERT(reader->isEndElement());
    nextElementDelim();
}

template <class T, class U>
void SimulationMaker::createTracesKindOf(const QList<TraceParam> &traceParam) {
    for (int i = 0; i < traceParam.size(); ++i) {
        const TraceParam &param( traceParam.value(i) );
        QString label = param.attribute("label").toString();
        QString value = param.attribute("value").toString();
        QList<T*> bases = seekMany<QObject*, T*>(value);
        // If several bases are found they will all get the same label.
        // This must be fixed, as needed, by Output
        for (int i = 0; i < bases.size(); ++i) {
            U *trace = new U(label, bases[i], param.parent);
            trace->appendAttributes(param);
        }
    }
}

void SimulationMaker::createTraces() {
    createTracesKindOf<PullVariableBase, Trace<PullVariableBase> >(traceVariableParam);
    createTracesKindOf<ParameterBase, Trace<ParameterBase> >(traceParameterParam);
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

} //namespace
