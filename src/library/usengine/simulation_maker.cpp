/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QtXml/QXmlStreamReader>
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
    outputDataParam.clear();

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
    QList<QObject*> models;
    for (int i = 0; i < parents.size(); ++i) {
        models << createModelElement(parents[i]);
    }
	nextElementDelim();
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
	Q_ASSERT(reader->isEndElement());
	nextElementDelim();
}

QList<QObject*> SimulationMaker::createModelElement(QObject *parent) {
    QString modelType = attributeValue("type", "anonymous");
    QString objectName = attributeValue("name", "anonymous");
    QString hide = attributeValue("hide", "");
    QString instancesStr = attributeValue("instances", "");

    int numInstances = 1;
    if (!instancesStr.isEmpty()) {
        bool ok(true);
        numInstances = instancesStr.toInt(&ok);
        if (!ok || numInstances <= 0)
            throw Exception(message("Attribute 'instances'' must a number larger than zero"));
    }

    QList<QObject*> models;
    Models instances;
    try {
        for (int i = 0; i < numInstances; ++i) {
            QString objectInstanceName = objectName;
            if (numInstances > 1)
                objectInstanceName += "(" + QString::number(i+1) + ")";

            Model *model = ModelMaker::create(modelType, objectInstanceName, parent);
            models << model;
            instances << model;

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
    QString name = attributeValue("name", parent);
    QString value = attributeValue("value", "");
    QString variableName = attributeValue("variable", "");
    QString dimensions = attributeValue("dimensions", "");

    bool hasValue = !value.isEmpty();
    bool hasVariable = !variableName.isEmpty();
    if (hasValue && hasVariable) {
        QString msg("Parameter '%1' cannot have both a 'value' and a 'variable' attribute");
        throw Exception(message(msg.arg(name)), parent);
    }
    if (!hasValue && !hasVariable) {
        QString msg("Parameter '%1' must have either a 'value' or a 'variable' attribute");
        throw Exception(message(msg.arg(name)), parent);
    }

    ParameterBase *parameter = seekOneChild<ParameterBase*>(name, parent);
    if (hasValue)
        parameter->setValueFromString(value.trimmed());
    else
        redirectedParameters.append(RedirectedParameter(parameter, variableName));
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
            readOutputParameterElement(output);
        }
        else if (elementNameEquals("variable")) {
            readOutputVariableElement(output);
        }
        else {
            throw Exception(message("Unexpected element: '" + elementName() + "'"), parent);
        }
	}	
	Q_ASSERT(reader->isEndElement());
	nextElementDelim();
}	

void SimulationMaker::readOutputVariableElement(QObject* parent)
{

    Q_ASSERT(reader->isStartElement() && parent);

    OutputParam param;
    param.label = attributeValue("label", parent),
    param.axis = attributeValue("axis", parent),
    param.value = attributeValue("value", parent);
    param.summary = attributeValue("summary", "");
    param.type = attributeValue("type", "");
    param.parent = parent;

    outputVariableParam.append(param);

    nextElementDelim();
    Q_ASSERT(reader->isEndElement());
    nextElementDelim();
}

void SimulationMaker::setupOutputVariableElements()
{
    for (int i = 0; i < outputVariableParam.size(); ++i) {
        OutputParam param = outputVariableParam[i];
        QList<PullVariableBase*> variables = seekMany<QObject*, PullVariableBase*>(param.value);
        for (int j = 0; j < variables.size(); ++j){
            new OutputVariable(param.label, param.axis, param.summary, variables[j], param.parent);
        }
    }
}

void SimulationMaker::readOutputParameterElement(QObject* parent)
{

    Q_ASSERT(reader->isStartElement() && parent);

    OutputParam param;
    param.label = attributeValue("label", parent),
    param.axis = attributeValue("axis", parent),
    param.value = attributeValue("value", parent);
    param.parent = parent;

    outputParameterParam.append(param);

    nextElementDelim();
    Q_ASSERT(reader->isEndElement());
    nextElementDelim();
}

void SimulationMaker::setupOutputParameterElements()
{
    for (int i = 0; i < outputParameterParam.size(); ++i) {
        OutputParam param = outputParameterParam[i];
        QList<ParameterBase*> parameters = seekMany<QObject*, ParameterBase*>(param.value);
        for (int i = 0; i < parameters.size(); ++i)
            new OutputParameter(param.label, param.axis, parameters[i], param.parent);
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
            QString msg("The type of variable '%1'does not match that of the parameter");
            throw Exception(message(msg.arg(variableName)), parameter);
        }
    }
}

} //namespace
