/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_DOCUMENTATION_WRITER
#define UNISIM_DOCUMENTATION_WRITER

#include <QFile>
#include <QList>
#include <usbase/identifier.h>

namespace UniSim{

class Model;
class NamedObject;
class FactoryPlugIn;
	
class DocumentationWriter
{
public:
    DocumentationWriter();
    void write();

private:
    // data
	QFile file;


    // methods
    void openFile();
    NamedObject *createModel(FactoryPlugIn *plugin, Identifier modelId);
    void writePlugins();
    void writeTimeStamp();
    void writeVersion();
    void write(FactoryPlugIn *plugin);
    void writeAuthors(FactoryPlugIn *plugin);
    void writeAuthor(QString id);
    void writeModels(FactoryPlugIn *plugin);
    void writeModel(FactoryPlugIn *plugin, Identifier modelId);
    void writeParameters(NamedObject *model);
    void writeVariables(NamedObject *model);
    void writeTableTitle(QString title);
    void writeTableRow(QString format, QString a, QString b, QString c);
    void write(QString s);
    void write(const char *s);

};

} //namespace


#endif
