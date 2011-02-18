/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QDir>
#include <QList>
#include <QtAlgorithms>
#include <QMessageBox>
#include <usbase/exception.h>
#include <usbase/file_locations.h>
#include <usbase/model.h>
#include <usbase/model_maker_plug_in.h>
#include <usbase/parameter.h>
#include <usbase/pull_variable_base.h>
#include <usbase/push_variable_base.h>
#include <usbase/utilities.h>
#include <usbase/version.h>
#include "documentation_writer.h"
#include "model_maker.h"

namespace {
    QString desc(QString s) {
        QString res = s.simplified();
        if (res.right(1) != ".")
            res += ".";
        res += "\n";
        return res;
    }

    QString index(UniSim::Identifier id) {
        return " " + id.key() + " @Index {" + id.label() + "} ";
    }
}

namespace UniSim{

DocumentationWriter::DocumentationWriter()
{
}

const char HEAD[] =
    "@BeginSubAppendices\n";

const char TAIL[] =
    "@EndSubAppendices\n"
    "@End @Appendix\n";

const char TABLE_BEGIN[] =
    "@LP\n"
    "@Tbl\n"
    "width { expand }\n"
    "strut { no }\n"
    "aformat { @Cell width {4c} A | @Cell width {3c} B | @Cell width {expand }C }\n"
    "bformat { @StartHSpan @Cell marginvertical {0.4f} paint {lightgrey}  @S A | @HSpan | @HSpan}\n"
    "{\n";

const char TABLE_END[] =
    "}\n";

const char FORMAT_FIRST_ROW[] =
    "marginabove {0.7f}\n";

const char FORMAT_LAST_ROW[] =
    "marginbelow {0.7f}\n";

const char FORMAT_VERY_LAST_ROW[] =
    "rulebelow { yes }\n";

const QString F("{Courier Base -2p} @Font ");

void DocumentationWriter::write() {
	openFile();
    writeTimeStamp();
    writeVersion();
    write(HEAD);
    writePlugins();
    write(TAIL);
    file.close();
}

void DocumentationWriter::openFile() {
    QDir dir = FileLocations::location(FileLocationInfo::Output);
    QString fileName = dir.absolutePath() + "/ms_generated.txt";
    file.setFileName(fileName);
    bool ok = file.open(QIODevice::Text | QIODevice::WriteOnly);
    if (!ok)
		throw Exception("Could not open file to write documentation:\n" + fileName);
}

void DocumentationWriter::writeTimeStamp() {
    QString timeStamp = "#Generated on " +
                        QDateTime::currentDateTime().toString("d MMM yy h:mm") + "\n";
    write(timeStamp);
}

void DocumentationWriter::writeVersion() {
    write("@LP Version: ");
    write(version());
    write(".");
}

namespace {
    bool pluginLessThan(const ModelMakerPlugIn* p1,
                        const ModelMakerPlugIn* p2) {
        return p1->pluginName().key() < p2->pluginName().key();
    }
}

void DocumentationWriter::writePlugins() {
    QList<ModelMakerPlugIn*> plugins = ModelMaker::plugins();
    qSort(plugins.begin(), plugins.end(), pluginLessThan);
    for (int i = 0; i < plugins.size(); ++i) {
        QString beginning = plugins[i]->pluginDesc().left(8);
        if (beginning.toLower() == "!exclude") continue;
        write(plugins[i]);
    }
}

void DocumentationWriter::write(ModelMakerPlugIn *plugin) {
    Identifier id = plugin->pluginName();
    write("@SubAppendix\n");
    write("@Title {" + id.label() + " plugin}\n");
    write("@Tag {" + id.key() + ".plugin}\n");
    write("@Begin @LP\n");
    write(index(id));
    write(desc(plugin->pluginDesc()));
    writeAuthors(plugin);
    writeModels(plugin);
    write("@NP @End @SubAppendix\n");
}

void DocumentationWriter::writeAuthors(ModelMakerPlugIn *plugin) {
    QStringList authors = plugin->authors();
    int n = authors.size();
    write("@LP Author");
    if (n > 1) write("s");
    write(": ");
    write(authors[0]);
    for (int i = 1; i < n-1; ++i) {
        write("; ");
        write(authors[i]);
    }
    if (n > 1) {
        write (" and ");
        write(authors[n-1]);
    }
    write(".\n");
}

void DocumentationWriter::writeModels(ModelMakerPlugIn *plugin) {
    write("@BeginSubSubAppendices\n");
    Identifiers ids = plugin->supportedClasses().keys();
    qSort(ids);
    for (int i = 0; i < ids.size(); ++i) {
        writeModel (plugin, ids[i]);

    }
    write("@EndSubSubAppendices\n");
}

void DocumentationWriter::writeModel(ModelMakerPlugIn *plugin, Identifier modelId) {
    QString modelName = plugin->pluginName().label() + "::" + modelId.label();
    Model *model = createModel(plugin, modelId);
    write("@SubSubAppendix\n");
    write("@Title {" + modelName + "}\n");
    write("@Tag {" + modelName + "}\n");
    write("@Begin @LP\n");
    write(index(modelId));
    write(plugin->supportedClasses().value(modelId) + "\n");
    write(TABLE_BEGIN);
    writeParameters(model);
    writePushVariables(model);
    writePullVariables(model);
    write(TABLE_END);
    write("@End @SubSubAppendix\n");
    delete model;
}

Model* DocumentationWriter::createModel(ModelMakerPlugIn *plugin, Identifier modelId) {
    Model *model = plugin->create(modelId, "anonymous");
    Q_ASSERT_X(model,
               "DocumentationWriter::createModel",
               qPrintable(plugin->pluginName().label() +
               " cannot create " + modelId.label()));
    /*
    try {
        model->initialize();
    }
    catch (Exception &ex) {
        //ignore
    }
    */
    return model;
}

// Refactor the three methods below when they have been given a common base class
// Notice use of FORMAT_LAST_ROW and FORMAT_VERY_LAST_ROW
void DocumentationWriter::writeParameters(Model *model) {
    writeTableTitle("Parameters");
    QList<ParameterBase*> params = seekChildren<ParameterBase*>("*", model);
    int n = params.size();
    if (n == 0)  {
        QString format = QString(FORMAT_FIRST_ROW) + FORMAT_LAST_ROW;
        writeTableRow(format, "", "", "@I None");
    }
    for (int i = 0; i < n; ++i) {
        Identifier id = params[i]->id();
        QString value = params[i]->toVariant().toString();
        QString format;
        if (i == 0)
            format = FORMAT_FIRST_ROW;
        else if (i == n-1)
            format = FORMAT_LAST_ROW;
        writeTableRow(format, id.label() + index(id), value, desc(params[i]->description()));
    }
}

void DocumentationWriter::writePushVariables(Model *model) {
    writeTableTitle("Push variables");
    QList<PushVariableBase*> var = model->seekChildren<PushVariableBase*>("*");

    int n = var.size();
    if (n == 0)  {
        QString format = QString(FORMAT_FIRST_ROW) + FORMAT_LAST_ROW;
        writeTableRow(format, "", "", "@I None");
    }
    for (int i = 0; i < n; ++i) {
        QString format;
        if (i == 0)
            format = FORMAT_FIRST_ROW;
        else if (i == n-1)
            format = FORMAT_LAST_ROW;
        writeTableRow(format, var[i]->id().label() + index(var[i]->id()), var[i]->typeId(), desc(var[i]->description()));
    }
}

void DocumentationWriter::writePullVariables(Model *model) {
    writeTableTitle("Pull variables");
    QList<PullVariableBase*> var = model->seekChildren<PullVariableBase*>("*");

    int n = var.size();
    if (n == 0)  {
        QString format = QString(FORMAT_FIRST_ROW) + FORMAT_VERY_LAST_ROW;
        writeTableRow(format, "", "", "@I None");
    }
    for (int i = 0; i < n; ++i) {
        QString format;
        if (i == 0)
            format = FORMAT_FIRST_ROW;
        else if (i == n-1)
            format = FORMAT_VERY_LAST_ROW;
        writeTableRow(format, var[i]->id().label() + index(var[i]->id()), var[i]->typeId(), desc(var[i]->description()));
    }
}

void DocumentationWriter::writeTableTitle(QString title) {
    write("@Rowb\n");
    write("ruleabove { yes }\n");
    write("rulebelow { yes }\n");
    write("A {" + title +"}\n");
}

void DocumentationWriter::writeTableRow(QString format, QString a, QString b, QString c) {
    write("@Rowa\n");
    write(format);
    write("A {" + F + a + "}\n");
    if (!a.isEmpty() && b.isEmpty())
        write("B {@I {empty string}}\n");
    else
        write("B {"+ F + "{" + b + "}}\n");
    write("C {" + c + "}\n");
}

void DocumentationWriter::write(QString s) {
    file.write(qPrintable(s));
}

void DocumentationWriter::write(const char *s) {
    file.write(s);
}

} //namespace

