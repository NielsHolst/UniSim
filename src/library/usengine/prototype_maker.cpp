/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <usbase/exception.h>
#include <usbase/file_locations.h>
#include <usbase/random_uniform.h>
#include "confirmation.h"
#include "prototype_maker.h"
#include "xml_elements_reader.h"

namespace {

    QString makeClassName(QString userClassName) {
        QString t;
        QString s = userClassName.simplified();
        bool capitalizeNext = true;
        for (int i = 0; i < s.size(); ++i) {
            QString next = QString(s[i]);
            bool isBlank = next == " ";
            if (!isBlank)
                t += capitalizeNext ? next.toUpper() : next;
            capitalizeNext = isBlank;
        }
        return t;
    }

    QString makeFileName(QString userClassName) {
        QString t;
        QString s = makeClassName(userClassName);
        for (int i = 0; i < s.size(); ++i) {
            QString next = QString(s[i]);
            bool isCap = next.toUpper() == next;
            if (isCap && i > 0)
                t += "_";
            t += next.toLower();
        }
        return t;
    }

    void emptyFolder(QDir dir) {
        foreach (QString fileName, dir.entryList(QDir::Files)) {
            dir.remove(fileName);
        }
    }
}

namespace UniSim{
	
PrototypeMaker::PrototypeMaker()
    : QObject(), destFolderPath() {
    reader = new XmlElementsReader;
    pluginTypeNames[ModelPlugin] = "models";
    pluginTypeNames[IntegratorPlugin] = "integrators";
}

PrototypeMaker::~PrototypeMaker() {
	delete reader;
}

void PrototypeMaker::parse(QString filePath) {
    classes.clear();
    inputFilePath = filePath;
    reader->open(filePath);
    reader->next();
    if (!reader->isEndDocument()) readPlugin();
    if (!reader->isEndDocument()) reader->hurl("End of XML document expected");
    writeOutput();
}

void PrototypeMaker::readPlugin() {
    if (reader->name() != "plugin") reader->hurl("Root element must be 'plugin'");
    QString type = reader->attributes().value("type").toString().simplified();
    QString name = reader->attributes().value("name").toString().simplified();
    if (type.isEmpty()) reader->hurl("'plugin' element misses 'type' attribute");
    if (name.isEmpty()) reader->hurl("'plugin' element misses 'name' attribute");

    type = type.toLower();
    if (type == "model")
        pluginType = ModelPlugin;
    else if (type == "integrator")
        pluginType = IntegratorPlugin;
    else
        reader->hurl("'plugin' type must be 'model' or 'integrator'");
    pluginName = name;
    classMakerName = makeClassName(pluginName) + "ModelMaker";

    reader->next();
    while (reader->isStartTag()) {
        if (reader->name() == "class")
            readClass();
        else
            reader->hurl("Unknown element in 'plugin' element: " + reader->name());
    }
    if (reader->isEndTag()) reader->next();
}

void PrototypeMaker::readClass() {
    QString name = reader->attributes().value("name").toString().simplified();
    if (name.isEmpty()) reader->hurl("'class' element misses 'name' attribute");
    classes[name] = ObjectNames();

    reader->next();
    while (reader->isStartTag()) {
        if (reader->name() == "model")
            readModel(name);
        else
            reader->hurl("Unknown element in 'class' element: " + reader->name());
    }
    reader->next();
}

void PrototypeMaker::readModel(ClassName className) {
    QString name = reader->attributes().value("name").toString().simplified();
    if (name.isEmpty()) reader->hurl("'model' element misses 'name' attribute");
    Q_ASSERT(classes.contains(className));
    classes[className] << name;
    reader->next();
    if (reader->isStartTag()) reader->hurl("No elements allowed inside 'model' elements");
    reader->next();
}

void PrototypeMaker::writeOutput() {
    createDestFolder();
    for (int i = 0; i < classes.size();++i) {
        writeClassHeaderFile(classes.keys()[i]);
        writeClassSourceFile(classes.keys()[i]);
    }
    writeMakerHeaderFile();
    writeMakerSourceFile();
    writeProjectFile();
    if (pluginType == ModelPlugin)
        WriteXmlModelFile();
}

void PrototypeMaker::createDestFolder() {
    QDir dir = FileLocations::location(FileLocations::Output);
    QString folderPath = dir.absolutePath();
    destFolderPath = folderPath + "/" + pluginName + "_" + pluginTypeNames[pluginType];

    QDir destDir(destFolderPath);
    if (destDir.exists()) {
        Confirmation confirmation("Destination folder already exists",
                                  "Overwrite existing folder?\n\nExisting folder: " + destFolderPath);
        if (!confirmation.accepted(true)) return;
        emptyFolder(destDir);
    }

    if (!QDir().mkpath(destFolderPath) || !destDir.exists()) {
        reader->hurl("Could not create destination folder:\n\n" + destFolderPath);
    }
}

bool PrototypeMaker::openOutputFile(QString fileName, QFile *file) const {
    QString path = destFolderPath + "/" + fileName;
    file->setFileName(path);
    bool ok = file->open(QIODevice::WriteOnly | QIODevice::Text);
    if (!ok) throw UniSim::Exception("Could not write prototype to file: '" + path + "'");
    return ok;
}

void PrototypeMaker::writeClassHeaderFile(ClassName className) const {
    QString fileName = makeFileName(className) + ".h";
    QFile file;
    if (!openOutputFile(fileName, &file)) return;

    QString cppCode;
    QTextStream text(&cppCode);

    QString guard = pluginName + "_" + className;
    guard = guard.toUpper().replace(".", "_");
    text
        << copyrightNotice()
        << "#ifndef " << guard << '\n'
        << "#define " << guard << '\n'
        << "#include <QObject>" << '\n'
        << "#include <usbase/model.h>" << '\n'
        << namespaceBegin()
        << "class " << className << " : public UniSim::Model" << '\n'
        << "{" << '\n'
        << "\tQ_OBJECT"<< '\n'
        << "public: " << '\n'
        << "\t" << className << "(UniSim::Identifier name, QObject *parent=0);" << '\n'
        << "\t// standard methods" << '\n'
        << "\tvoid initialize();" << '\n'
        << "\tvoid reset();" << '\n'
        << "\tvoid update();" << '\n'
        << '\n'
        << "private:" << '\n'
        << "\t// parameters" << '\n'
        << "\tdouble Ninit, K, r;" << '\n'
        << '\n'
        << "\t// state" << '\n'
        << "\tdouble density;" << '\n'
        << '\n'
        << "};" << '\n'
        << '\n'
        << "} //namespace" << '\n'
        << "#endif" << '\n';

    file.write(qPrintable(cppCode));
}

QString PrototypeMaker::copyrightNotice() const
{
    return
    "/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.\n"
    "** Copyrights reserved.\n"
    "** Released under the terms of the GNU General Public License version 3.0 or later.\n"
    "** See www.gnu.org/copyleft/gpl.html.\n"
    "*/\n";

}

QString PrototypeMaker::namespaceBegin() const {
    return "\nnamespace " + pluginName + " {\n\n";
}

void PrototypeMaker::writeClassSourceFile(ClassName className) const{
    QString fileName = makeFileName(className) + ".cpp";
    QFile file;
    if (!openOutputFile(fileName, &file)) return;

    QString cppCode;
    QTextStream text(&cppCode);
    text
        << copyrightNotice()
        << "#include <usbase/pull_variable.h>\n"
        << "#include \"" << makeFileName(className) << ".h\"" << '\n'
        << "\nusing namespace UniSim;\n\n"
        << namespaceBegin()
        << className << "::" << className << "(UniSim::Identifier name, QObject *parent)" << '\n'
        << "\t: Model(name, parent) {" << '\n'
        << "\tnew PullVariable(\"N\", &density, this, \"Description\");" << '\n'
        << "}" << '\n'
        << '\n'
        << "void " << className << "::initialize() {" << '\n'
        << "\tsetParameter(\"Ninit\", &Ninit, 1., \"Description\");" << '\n'
        << "\tsetParameter(\"K\", &K, 1000., \"Description\");" << '\n'
        << "\tsetParameter(\"r\", &r, 1.2, \"Description\");" << '\n'
        << "}" << '\n'
        << '\n'
        << "void " << className << "::reset() {" << '\n'
        << "\tdensity = Ninit;" << '\n'
        << "}" << '\n'
        << '\n'
        << "void " << className << "::update() {" << '\n'
        << "\tdensity += (K<=0) ? 0 : density*r*(K-density)/K;" << '\n'
        << "}" << '\n'
        << '\n'
        << "} //namespace" << '\n'
        << '\n';
    file.write(qPrintable(cppCode));
}

void PrototypeMaker::writeMakerHeaderFile() const {
    QString fileName = pluginName + "_model_maker.h";
    QFile file;
    if (!openOutputFile(fileName, &file)) return;

    QString cppCode;
    QTextStream text(&cppCode);

    QString guard = fileName.toUpper().replace(".", "_");
    text
        << copyrightNotice()
        << "#ifndef " << guard << '\n'
        << "#define " << guard << '\n'
        << '\n'
        << "#include <usbase/model_maker_plug_in.h>" << "\n\n"
        << "namespace UniSim {" << "\n\n"
        << "\tclass ObjectPool;" << "\n\n"
        << "}\n\n"
        << namespaceBegin()
        << "class " << classMakerName << " : public QObject, public UniSim::ModelMakerPlugIn" << '\n'
        << "{" << '\n'
        << "\tQ_OBJECT" << '\n'
        << "\tQ_INTERFACES(UniSim::ModelMakerPlugIn)" << '\n'
        << "public:" << '\n'
        << "\tUniSim::Identifier pluginName() const;" << '\n'
        << "\tQString pluginDesc() const;" << '\n'
        << "\tQStringList authors() const;" << '\n'
        << "\tconst QMap<UniSim::Identifier, QString>& supportedClasses();" << '\n'
        << "\tvoid useObjectPool(UniSim::ObjectPool *pool) const;" << '\n'
        << "\tUniSim::Model* create(UniSim::Identifier modelType," << '\n'
        << "\t                      UniSim::Identifier objectName," << '\n'
        << "\t                      QObject *parent=0);" << '\n'
        << "};" << '\n'
        << '\n'
        << "} //namespace" << '\n'
        << '\n'
        << "#endif" << '\n'
        << '\n';
    file.write(qPrintable(cppCode));
}

void PrototypeMaker::writeMakerSourceFile() const {
    QString baseFileName = pluginName + "_model_maker";
    QFile file;
    if (!openOutputFile(baseFileName + ".cpp", &file)) return;

    QString cppCode;
    QTextStream text(&cppCode);
    text 
        << copyrightNotice()
        << "#include <usbase/object_pool.h>\n"
        << "#include <usbase/utilities.h>\n"
        << "#include \"" << baseFileName << ".h\"" << '\n'
        << modelIncludes()
        << '\n'
        << "using namespace UniSim;" << '\n'
        << '\n'
        << "namespace " << pluginName << "{" << '\n'
        << '\n'

        << plugInDesc()
        << plugInName()
        << authors()
        << supportedClasses()
        << useObjectPool()
        << create()

        << '\n'
        << "Q_EXPORT_PLUGIN2(" << baseFileName << ", " << classMakerName << ")" << '\n'
        << '\n'
        << "} //namespace" << '\n';
    file.write(qPrintable(cppCode));
}

QString PrototypeMaker::plugInDesc() const {
    return
    "QString " + classMakerName + "::pluginDesc() const\n"
    "{\n"
        "\treturn \"Description of " + pluginName + "\";\n"
    "}\n\n";
}

QString PrototypeMaker::authors() const {
    return
    "QStringList " + classMakerName + "::authors() const\n"
    "{\n"
        "\treturn QStringList() << \"author1\" << \"author2\";\n"
    "}\n\n";
}

QString PrototypeMaker::supportedClasses() const {
    return
    "const QMap<Identifier, QString>& " + classMakerName +
    "::supportedClasses()\n"
    "{\n"
    "\tif (!desc.isEmpty()) return desc;" +
    classDesc() +
    "\n\n\treturn desc;\n"
    "}\n\n";
}

QString PrototypeMaker::classDesc() const {
    QString s;
    for (int i = 0; i < classes.size(); ++i) {
        QString name = classes.keys()[i];
        s += "\n\n\tdesc[\"" + name + "\"] =\n"
             "\t\"Description\";";
    }
    return s;
}

QString PrototypeMaker::create() const {
    return
    "Model* " + classMakerName +
    "::create(Identifier modelType, Identifier objectName, QObject *parent)\n"
    "{\n"
    "\tsetSimulationObjectFromDescendent(parent);\n"
    "\tModel *model = 0;\n" +
    modelCreation() +
    "\treturn model;\n"
    "}\n\n";
}

QString PrototypeMaker::modelIncludes() const {
    QString s;
    for (int i = 0; i < classes.size(); ++i)
        s += "#include \"" + makeFileName(classes.keys()[i]) + ".h\"" + '\n';
    return s;
}

QString PrototypeMaker::plugInName() const {
    QString s;
    s = "Identifier " + classMakerName + "::pluginName() const {\n";
    s += "\treturn \""+ pluginName + "\";\n";
    s += "}\n\n";
    return s;
}

QString PrototypeMaker::useObjectPool() const {
    QString s;
    s = "void " + classMakerName + "::useObjectPool(ObjectPool *pool) const {\n";
    s += "\tobjectPool()->deferTo(pool);\n";
    s += "}\n";
    return s;
}


QString PrototypeMaker::modelIdentifiers() const {
    QString s;
    for (int i = 0; i < classes.size(); ++i)
        s += "\t\t<< Identifier(\"" + classes.keys()[i] + "\")" + '\n';
    s += ";\n";
    return s;
}

QString PrototypeMaker::modelCreation() const {
    QString s;
    for (int i = 0; i < classes.size(); ++i) {
        s += "\t";
        if (i > 0) s += "else ";
        s += "if (modelType.equals(\"" + classes.keys()[i].toLower() + "\"))\n";
        s += "\t\tmodel = new " + classes.keys()[i] + "(objectName, parent);\n";
    }
    return s;
}

void PrototypeMaker::writeProjectFile() const {
    QString projectName = pluginName + "_" + pluginTypeNames[pluginType];
    QString fileName = projectName + ".pro";
    QFile file;
    if (!openOutputFile(fileName, &file)) return;

    QString cppCode;
    QTextStream text(&cppCode);
    text
        << "include(../../config.pri)" << '\n'
        << '\n'
        << "TEMPLATE = lib" << '\n'
        << "DESTDIR = $${US_PLUGINS}" << '\n'
        << "TARGET = " << projectName << "_$${UNISIM_VERSION}$${DEBUG_SUFFIX}" << '\n'
        << "CONFIG += plugin" << '\n'
        << '\n'
        << "include ($${US_BASE}/base.pri)" << '\n'
        << '\n'
        << "HEADERS += \\" << '\n'
        << projectFiles(".h") << '\n'
        << "SOURCES += \\" << '\n'
        << projectFiles(".cpp")
        << '\n';
    file.write(qPrintable(cppCode));
}

QString PrototypeMaker::projectFiles(QString extension) const {
    QString s;
    int n = classes.size();

    s += "\t" + pluginName + "_model_maker" + extension;
    s += (n == 0) ? "\n" :" \\\n";

    for (int i = 0; i < n; ++i) {
        s += "\t" + makeFileName(classes.keys()[i]) + extension;
        if (i < n-1) s += " \\";
        s += '\n';
    }
    return s;
}

void PrototypeMaker::WriteXmlModelFile() const {
    QString fileName = pluginName + ".xml";
    QFile file;
    if (!openOutputFile(fileName, &file)) return;

    QString cppCode;
    QTextStream text(&cppCode);
    text
        << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>" << '\n'
        << "<simulation version=\"1.0\">" << '\n'
        << "\t<integrator type=\"TimeStepLimited\">" << '\n'
        << "\t\t<parameter name=\"maxSteps\" value=\"60\"/>" << '\n'
        << "\t\t<sequence>" << '\n'
        << xmlModelNames()
        << "\t\t</sequence>" << '\n'
        << "\t</integrator>" << '\n'
        << xmlModelClasses() << '\n'
        << xmlOutputs()
        << "</simulation>" << '\n';
    file.write(qPrintable(cppCode));
}

QString PrototypeMaker::xmlModelNames() const {
    QString s;
    s += "\t\t\t<model name=\"calendar\"/>\n";
    for (Classes::const_iterator cl = classes.begin(); cl != classes.end(); ++cl) {
        const ObjectNames &objectNames(cl.value());
        for (int i = 0; i < objectNames.size(); ++i)
            s += "\t\t\t<model name=\"" + objectNames[i] +"\"/>\n";
    }
    return s;
}

QString PrototypeMaker::xmlModelClasses() const {
    QString s;
    s += "\n\t<model name=\"calendar\" type=\"Calendar\">\n"
         "\t\t<parameter name=\"firstDate\" value=\"1/1/2010\"/>\n"
         "\t</model>\n";
    for (Classes::const_iterator cl = classes.begin(); cl != classes.end(); ++cl) {
        const ObjectNames &objectNames(cl.value());
        for (int i = 0; i < objectNames.size(); ++i) {
            s += "\n\t<model name=\"" + objectNames[i] + "\" type=\"" + pluginName + "::" + cl.key() + "\">\n";
            s += parameter("Ninit", 1, 5);
            s += parameter("r", 1.02, 1.12);
            s += parameter("K", 70, 150);
            s += "\t</model>\n";
        }
    }
    return s;
}

QString PrototypeMaker::xmlOutputs() const {
    QString s;
    for (Classes::const_iterator cl = classes.begin(); cl != classes.end(); ++cl) {
        const ObjectNames &objectNames(cl.value());
        for (int i = 0; i < objectNames.size(); ++i) {
            s += "\t<output type=\"plot\">\n"
                 "\t\t<parameter name=\"title\" value =\"" +
                 makeClassName(objectNames[i]) + "\"/>\n"

                 "\t\t<variable axis=\"x\" label=\"Day\" value=\"calendar[dayInYear]\"/>\n"
                 "\t\t<variable axis=\"y\" label=\"Density\" value=\"" +
                 objectNames[i] + "[N]\"/>\n"
                 "\t</output>\n";
        }
    }
    return s;
}


QString PrototypeMaker::parameter(QString name, double minValue, double maxValue) const {
    double value = minValue + (maxValue - minValue)*randomUniform()->next();
    QString s;
    s += "\t\t<parameter name=\"" + name + "\" value=\"" + QString::number(value) + "\"/>\n";
    return s;
}

QString PrototypeMaker::destinationFolder() const {
    return destFolderPath;
}

} //namespace
