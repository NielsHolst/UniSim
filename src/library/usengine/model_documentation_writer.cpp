/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QtAlgorithms>
#include <QXmlStreamWriter>
#include <usbase/model.h>
#include <usbase/utilities.h>
#include "model_documentation_writer.h"
#include "model_maker.h"


namespace UniSim{

ModelDocumentationWriter::ModelDocumentationWriter()
    : device(0)
{
}

void ModelDocumentationWriter::setDevice(QIODevice * device_) {
    device = device_;
}

void ModelDocumentationWriter::write() {
    Q_ASSERT(device);
    xml = new QXmlStreamWriter(device);
    xml->setAutoFormatting(true);
    xml->setCodec("ISO-8859-1");

    xml->writeStartDocument();
    xml->writeStartElement("html");

    setupIds();
    writeContents();
    writeByPlugIn();
    delete xml;
}

void ModelDocumentationWriter::setupIds() {
    ids.clear();
    Identifiers all = ModelMaker::selection();
    for (int i = 0; i < all.size(); ++i) {
        if (all[i].key().contains("::"))
            ids.append(all[i]);
    }
    qSort(ids.begin(), ids.end());
}


void ModelDocumentationWriter::writeContents() {
    xml->writeStartElement("h1");
    xml->writeCharacters("Model plug-ins");
    xml->writeEndElement();

    QString curNs;
    for (int i = 0; i < ids.size(); ++i) {
        QString ns, model;
        splitAtNamespace(ids[i].label(), &ns, &model);
        if (ns != curNs) {
            xml->writeStartElement("p");

            xml->writeStartElement("a");
            xml->writeAttribute("href", "#" + ns);
            xml->writeEndElement();

            xml->writeCharacters(ns);
            xml->writeEndElement();
            curNs = ns;
        }
    }
}

void ModelDocumentationWriter::writeByPlugIn() {
    QString curNs;
    for (int i = 0; i < ids.size(); ++i) {
        QString ns, modelName;
        splitAtNamespace(ids[i].label(), &ns, &modelName);
        if (ns != curNs) {
            xml->writeStartElement("h1");

            xml->writeStartElement("a");
            xml->writeAttribute("name", ns);
            xml->writeEndElement();

            xml->writeCharacters(ns);
            xml->writeEndElement();
            curNs = ns;
        }

        xml->writeStartElement("h2");
        xml->writeCharacters(modelName);
        xml->writeEndElement();
        Model *model = createModel(ids[i]);
        writeStates(model);
        writeParameters(model);
        delete model;
    }
}

Model* ModelDocumentationWriter::createModel(Identifier id) {
    Model *model = ModelMaker::create(id, "anonymous");
    try {
        model->initialize();
    }
    catch (Exception &ex) {
        //ignore
    }
    return model;
}

void ModelDocumentationWriter::writeStates(const Model *model) {
    xml->writeStartElement("h3");
    xml->writeCharacters("State");
    xml->writeEndElement();

    Model::States states = model->states();
    for (Model::States::const_iterator st = states.begin();
        st != states.end(); ++st)
    {
        xml->writeStartElement("p");
        xml->writeCharacters(st.key().label());
        xml->writeEndElement();
    }
    if (states.isEmpty())
    {
        xml->writeStartElement("p");
        xml->writeCharacters("(none)");
        xml->writeEndElement();
    }
}

void ModelDocumentationWriter::writeParameters(const Model *model) {
    xml->writeStartElement("h3");
    xml->writeCharacters("Parameters");
    xml->writeEndElement();

    Identifiers params = model->allParameters();
    for (int i = 0; i < params.size(); ++i) {
        Identifier id = params[i];
        QString value = model->parameterAsString(id);
        xml->writeStartElement("p");
        xml->writeCharacters(id.label() + " \"" + value + "\"");
        xml->writeEndElement();
    }
    if (params.isEmpty())
    {
        xml->writeStartElement("p");
        xml->writeCharacters("(none)");
        xml->writeEndElement();
    }
}

} //namespace

