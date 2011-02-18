/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
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
class ModelMakerPlugIn;
	
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
    Model* createModel(ModelMakerPlugIn *plugin, Identifier modelId);
    void writePlugins();
    void writeTimeStamp();
    void writeVersion();
    void write(ModelMakerPlugIn *plugin);
    void writeAuthors(ModelMakerPlugIn *plugin);
    void writeModels(ModelMakerPlugIn *plugin);
    void writeModel(ModelMakerPlugIn *plugin, Identifier modelId);
    void writeParameters(Model *model);
    void writePushVariables(Model *model);
    void writePullVariables(Model *model);
    void writeTableTitle(QString title);
    void writeTableRow(QString format, QString a, QString b, QString c);
    void write(QString s);
    void write(const char *s);

};

} //namespace


#endif
