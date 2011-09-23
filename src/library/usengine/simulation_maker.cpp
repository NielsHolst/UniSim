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
#include <usbase/dataset.h>
#include <usbase/integrator.h>
#include <usbase/exception.h>
#include <usbase/output.h>
#include <usbase/output_data.h>
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
    if (!file.open(QIODevice::ReadOnly)) throw Exception(message("Cannot open file: '"+fileName+"' for reading."));
	reader->setDevice(&file);

    if (!nextElementDelim()) throw Exception(message("File is not in valid XML format"));
    if (elementNameNotEquals("simulation")) throw Exception(message("Root element must be 'simulation'"));

    simName = attributeValue("name", "anonymous");
    simVersion = attributeValue("version", "1.0");
	
	Simulation *sim = new Simulation(simName, simVersion);
    UniSim::setSimulationObject(sim);

	nextElementDelim();
    int iCon=0, iMod=0, iOut=0;
    while (!reader->hasError() && reader->isStartElement()) {
        if (elementNameEquals("integrator") || elementNameEquals("controller")) {
			if (readIntegratorElement(sim)) ++iCon;
        } else if (elementNameEquals("model")) {
			if (readModelElement(sim)) ++iMod;
        } else if (elementNameEquals("output")) {
			if (readOutputElement(sim)) ++iOut;
		} else {
            throw Exception(message("Unexpected element: '" + elementName() + "'"), sim);


		}
	}
	Q_ASSERT(reader->isEndElement());
    if (reader->hasError()) throw Exception(message(""));
    if (iCon==0) throw Exception(message("Missing 'integrator' element in 'simulation'"));
    else if (iCon>1) throw Exception(message("Only one 'integrator' element allowed in 'simulation'"));
    if (iMod==0) throw Exception(message("Missing 'model' element in 'simulation'"));
    if (iOut==0) throw Exception(message("Missing 'output' element in 'simulation'"));

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

bool SimulationMaker::readIntegratorElement(QObject* parent)
{
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
            readParameterElement(integrator);
        }
        else if (elementNameEquals("model")){
            readModelElement(integrator);
        }
        else {
            throw Exception(message(
                           "Unexpected element: '" + elementName() + "'"), parent);
		}
	}	
	Q_ASSERT(reader->isEndElement());
	nextElementDelim();
	
	return integrator;
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
            throw Exception(message(
                           "Unknown child element of 'sequence' element: " + elementName()), parent);
		}
		nextElementDelim();
		Q_ASSERT(reader->isEndElement());
		nextElementDelim();
	}	
	Q_ASSERT(reader->isEndElement());

	nextElementDelim();
}

bool SimulationMaker::readModelElement(QObject* parent)
{
	Q_ASSERT(reader->isStartElement());
	
    QString modelType = attributeValue("type", "anonymous");
    QString objectName = attributeValue("name", "anonymous");
    QString hide = attributeValue("hide", "");
    QString instancesStr = attributeValue("instances", "");

    bool manyInstances = !instancesStr.isEmpty();
    int instances = 1;
    if (manyInstances) {
        bool ok(true);
        instances = instancesStr.toInt(&ok);
        if (!ok || instances <= 0)
            throw Exception("instances must a number larger than zero");
    }

    Model *model;
    try {
        for (int i = 0; i < instances; ++i) {
            QString objectInstanceName = objectName;
            if (manyInstances)
                objectInstanceName += "(" + QString::number(i+1) + ")";

            model = ModelMaker::create(modelType, objectInstanceName, parent);

            if (!hide.isEmpty()) {
                bool isHidden = UniSim::stringToValue<bool>(hide);
                model->setHide(isHidden);
            }
        }
    }
    catch (Exception &ex) {
        throw Exception(message(ex.message()), parent);
    }

	nextElementDelim();
	while (!reader->hasError() && reader->isStartElement()) {
        if (elementNameEquals("model")) {
			readModelElement(model);
        }
        else if (elementNameEquals("dataset")){
            readDatasetElement(model);
        }
        else if (elementNameEquals("parameter")){
            readParameterElement(model);
        }
        else {
            throw Exception(message("Unexpected element: '" + elementName() + "'"), parent);
        }
	}	
	Q_ASSERT(reader->isEndElement());
	nextElementDelim();
	
	return model;
}

void SimulationMaker::readDatasetElement(QObject* parent)
{
    Q_ASSERT(reader->isStartElement());

    QString objectName = attributeValue("name", parent);

    Dataset *dataset = new Dataset(objectName, parent);

    nextElementDelim();
    while (!reader->hasError() && reader->isStartElement()) {
        if (elementNameEquals("parameter")){
            readParameterElement(dataset);
        }
        else {
            throw Exception(message("Unexpected element: '" + elementName() + "'"), parent);
        }
    }
    Q_ASSERT(reader->isEndElement());
    nextElementDelim();
}

void SimulationMaker::readParameterElement(QObject* parent)
{
    Q_ASSERT(reader->isStartElement() && parent);

    QString name = attributeValue("name", parent);
    QString value = attributeValue("value", "");
    QString variableName = attributeValue("variable", "");

    bool hasValue = !value.isEmpty();
    bool hasVariable = !variableName.isEmpty();
    if (hasValue && hasVariable)
        throw Exception("Parameter '" + name +
                        "' cannot have both a 'value' and a 'variable' attribute", parent);
    if (!hasValue && !hasVariable)
        throw Exception("Parameter '" + name +
                        "' must have either a 'value' or a 'variable' attribute", parent);

    ParameterBase *parameter = seekOneChild<ParameterBase*>(name, parent);
    if (hasValue)
        parameter->setValueFromString(value.trimmed());
    else
        redirectedParameters.append(RedirectedParameter(parameter, variableName));

	nextElementDelim();
	Q_ASSERT(reader->isEndElement());
	nextElementDelim();
}

bool SimulationMaker::readOutputElement(QObject* parent)
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
            readParameterElement(output);
        }
        else if (elementNameEquals("read-parameter")) {
            readOutputParameterElement(output);
        }
        else if (elementNameEquals("variable")) {
            readOutputVariableElement(output);
        }
        else if (elementNameEquals("data")) {
            readOutputDataElement(output);
        }
        else {
            throw Exception(message("Unexpected element: '" + elementName() + "'"), parent);
        }
	}	
	Q_ASSERT(reader->isEndElement());
	nextElementDelim();
    return output;
}	

void SimulationMaker::readOutputVariableElement(QObject* parent)
{

	Q_ASSERT(reader->isStartElement() && parent);

    OutputParam param;
    param.label = attributeValue("label", parent),
    param.axis = attributeValue("axis", parent),
    param.value = attributeValue("value", parent);
    param.summary = attributeValue("summary", "");
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

void SimulationMaker::readOutputDataElement(QObject* parent)
{

    Q_ASSERT(reader->isStartElement() && parent);

    OutputParam param;
    param.label = attributeValue("label", parent),
    param.axis = attributeValue("axis", parent),
    param.value = attributeValue("value", parent);
    param.parent = parent;

    outputDataParam.append(param);

    nextElementDelim();
    Q_ASSERT(reader->isEndElement());
    nextElementDelim();
}

void SimulationMaker::setupOutputDataElements()
{
    for (int i = 0; i < outputDataParam.size(); ++i) {
        OutputParam param = outputDataParam[i];
        QString datasetName, columnName;
        splitOutputDataValue(param.value, &datasetName, &columnName);

        Dataset *dataset = seekOne<Dataset*>(datasetName);
        if (!dataset->contains(columnName)) {
            QString msg = "Dataset '" + datasetName + "' has no column named '" + columnName + "'";
            throw Exception(message(msg));
        }
        new OutputData(param.label, param.axis, dataset, columnName, param.parent);
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
    if (!ok)
        throw Exception(message("Value badly formatted: " + value));
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
    if (result.isEmpty())
        throw Exception("Missing attribute '" + name + "'", parent);
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
        if (!coupled)
            throw Exception("The type of variable '" + variableName +
                            "' does not match that of the parameter", parameter);
    }
}

} //namespace
