/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef PROTOTYPE_MAKER_H
#define PROTOTYPE_MAKER_H
#include <QMap>
#include <QObject>
#include <QStringList>
#include <usbase/identifier.h>

class QFile;

namespace UniSim{

class XmlElementsReader;

class PrototypeMaker : public QObject
{
	Q_OBJECT
	
public:
    PrototypeMaker();
    ~PrototypeMaker();
    void parse(QString filePath);
    QString destinationFolder() const;
private:
    typedef QString ClassName;
    typedef QStringList ObjectNames;
    typedef QMap<ClassName, ObjectNames> Classes;

    void readPlugin();
    void readClass();
    void readModel(ClassName className);

    void writeOutput();
    void createDestFolder();
    bool openOutputFile(QString fileName, QFile *file) const;
    void writeClassHeaderFile(ClassName className) const;
    void writeClassSourceFile(ClassName className) const;
    void writeMakerHeaderFile() const;
    void writeMakerSourceFile() const;
    void writeProjectFile() const;
    void WriteXmlModelFile() const;
    QString copyrightNotice() const;
    QString namespaceBegin() const;
    QString modelIncludes() const;
    QString modelIdentifiers() const;
    QString modelPlugInName() const;
    QString modelUseObjectPool() const;
    QString modelCreation() const;
    QString projectFiles(QString extension) const;
    QString xmlModelClasses() const;
    QString xmlModelNames() const;
    QString xmlOutputs() const;
    QString parameter(QString name, double minValue, double maxValue) const;

    XmlElementsReader *reader;

    QString inputFilePath, destFolderPath, pluginName, classMakerName;
    Classes classes;

    enum PluginType {ModelPlugin, IntegratorPlugin};
    PluginType pluginType;
    QMap<PluginType, QString> pluginTypeNames;
};

}//namespace

#endif

