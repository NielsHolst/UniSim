/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_MODEL_DOCUMENTATION_WRITER
#define UNISIM_MODEL_DOCUMENTATION_WRITER

#include <QList>
#include <usbase/identifier.h>

class QIODevice;
class QXmlStreamWriter;

namespace UniSim{

class Model;
	
class ModelDocumentationWriter
{
public:
    ModelDocumentationWriter();
    void setDevice(QIODevice * device);
    void write();

private:
    // data
    QIODevice * device;
    QXmlStreamWriter *xml;
    QList<Identifier> ids;

    // methods
    void setupIds();
    Model* createModel(Identifier id);
    void writeContents();
    void writeByPlugIn();
    void writeStates(const Model *model);
    void writeParameters(const Model *model);
};

} //namespace


#endif
