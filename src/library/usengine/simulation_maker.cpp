/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
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
#include <usbase/output_variable.h>
#include <usbase/parameter_base.h>
#include <usbase/utilities.h>
#include "integrator_maker.h"
#include "model_maker.h"
#include "output_maker.h"
#include "simulation.h"
#include "simulation_maker.h"
#include "xml_expander.h"
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
	if (!reader->hasError() && !reader->isEndDocument()) {
		do {
			reader->readNext();
		} while (!reader->hasError() && !reader->isEndDocument() && 
				!reader->isStartElement() && !reader->isEndElement()); 
	}
	return reader->isStartElement() || reader->isEndElement();
}

Simulation* SimulationMaker::parse(QString fileName_)
{
    fileName = fileName_;
	QString simName, simVersion;

    XmlExpander expander(fileName, "_expanded");
	emit beginExpansion();
	expander.expand();
	emit endExpansion();	
    fileName = expander.newFileName();
	
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) throw Exception(message("Cannot open file: '"+fileName+"' for reading."));
	reader->setDevice(&file);

    if (!nextElementDelim()) throw Exception(message("File is not in valid XML format"));
    if (elementNameNotEquals("simulation")) throw Exception(message("Root element must be 'simulation'"));

    simName = attributeValue("name");
	if (simName.isEmpty()) simName = "anonymous";
    simVersion = attributeValue("version");
	if (simVersion.isEmpty()) simVersion = "1.0";
	
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
            throw Exception(message("Unknown element in 'simulation' element: " + elementName()));
		}
	}
	Q_ASSERT(reader->isEndElement());
    if (reader->hasError()) throw Exception(message(""));
    if (iCon==0) throw Exception(message("Missing 'integrator' element in 'simulation'"));
    else if (iCon>1) throw Exception(message("Only one 'integrator' element allowed in 'simulation'"));
    if (iMod==0) throw Exception(message("Missing 'model' element in 'simulation'"));
    if (iOut==0) throw Exception(message("Missing 'output' element in 'simulation'"));

	reader->clear();
    emit beginInitialization();
    sim->initialize(_sequence);
    emit endInitialization();

    return sim;
}

bool SimulationMaker::readIntegratorElement(QObject* parent)
{
	Q_ASSERT(reader->isStartElement() && parent);
	
    QString type = attributeValue("type");
	if (type.isEmpty())
        throw Exception(message("Missing 'type' attribute for 'integrator' element"));
 	
    QString name = attributeValue("name");
	if (name.isEmpty()) name = "anonymous";

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
        else {
            throw Exception(message(
                           "Unknown element in 'model' element: " + elementName()));
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
            QString model = attributeValue("name");
			if (model.isEmpty()) 
                throw Exception(message("Missing 'name' attribute of 'model' element in 'sequence"));
            _sequence.append(model);
        }
        else {
            throw Exception(message(
                           "Unknown child element of 'sequence' element: " + elementName()));
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
	
    QString modelType = attributeValue("type");
	if (modelType.isEmpty()) modelType = "anonymous";
	
    QString objectName = attributeValue("name");
	if (objectName.isEmpty()) objectName = "anonymous";

    Model *model;
    try {
        model = ModelMaker::create(modelType, objectName, parent);
    }
    catch (Exception &ex) {
        throw Exception(message(ex.message()));
    }

	nextElementDelim();
	while (!reader->hasError() && reader->isStartElement()) {
        if (elementNameEquals("model")) {
			readModelElement(model);
        }
        else if (elementNameEquals("parameter")){
            readParameterElement(model);
        }
        else {
            throw Exception(message(
                           "Unknown element in 'model' element: " + elementName()));
		}
	}	
	Q_ASSERT(reader->isEndElement());
	nextElementDelim();
	
	return model;
}

void SimulationMaker::readParameterElement(QObject* parent)
{
    Q_ASSERT(reader->isStartElement() && parent);

    QString name = attributeValue("name");
    QString value = attributeValue("value");
	
    if (name.isEmpty() || value.isEmpty())
        throw Exception(message("Missing name or value for parameter"), parent);

    ParameterBase *param = seekOneChild<ParameterBase*>(name, parent);
    param->setValueFromString(value.trimmed());

	nextElementDelim();
	Q_ASSERT(reader->isEndElement());
	nextElementDelim();
}

bool SimulationMaker::readOutputElement(QObject* parent)
{
	Q_ASSERT(reader->isStartElement() && parent);
	

    QString objectName = attributeValue("name");
	if (objectName.isEmpty()) objectName = "anonymous";
    QString type = attributeValue("type").toLower();

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
            readParameterElement(output);
        }
        else if (elementNameEquals("variable")) {
            readVariableElement(output);
        }
        else {
            throw Exception(message(
                    "Unknown element in 'output' element: " + elementName()));
		}
	}	
	Q_ASSERT(reader->isEndElement());
	nextElementDelim();
    return output;
}	

void SimulationMaker::readVariableElement(QObject* parent)
{
    /*
	Q_ASSERT(reader->isStartElement() && parent);

    QString label, axis, value;
    label = attributeValue("label");
    if (label.isEmpty())
        throw Exception(message("Missing 'label' attribute for 'variable' element"), parent);

    axis = attributeValue("axis");
    if (axis.isEmpty())
        throw Exception(message("Missing 'axis' attribute for 'variable' element"), parent);

    QString value = attributeValue("value");
    if (value.isEmpty())
        throw Exception(message("Missing 'value' attribute for 'variable' element"), parent);

    new OutputVariable(label, axis, value, parent);
*/

    OutputVariable::Raw raw;

    raw.label = attributeValue("label");
    if (raw.label.isEmpty())
        throw Exception(message("Missing 'label' attribute for 'variable' element"));

    raw.axis = attributeValue("axis");
    if (raw.axis.isEmpty())
        throw Exception(message("Missing 'axis' attribute for 'variable' element"));

    QString value = attributeValue("value");
    if (value.isEmpty())
        throw Exception(message("Missing 'value' attribute for 'variable' element"));

    QStringList parts = value.split("[");
    if (parts.size() != 2 || parts[1].right(1) != "]" || parts[1].size() == 1)
        throw Exception(message("Value of variable is not of the form 'x/y/z[state]' "));

    raw.modelName = parts[0];
    raw.stateNameInModel = parts[1].left(parts[1].size()-1);

    OutputVariable::appendVariable(raw, parent);

    nextElementDelim();
    Q_ASSERT(reader->isEndElement());
    nextElementDelim();
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

QString SimulationMaker::attributeValue(QString name) const {
    return reader->attributes().value(name).toString().trimmed();
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

} //namespace
