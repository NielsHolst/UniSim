/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INTERCOM_OUTPUT_FILE
#define INTERCOM_OUTPUT_FILE

#include <QFile>
#include <usbase/output.h>

namespace UniSim
{
    class Model;
}

namespace intercom{

class OutputFile : public UniSim::Output
{
	Q_OBJECT
public:
    OutputFile(UniSim::Identifier name, QObject *parent=0);
    // standard methods
    void initialize();
    void cleanup();
	
private:
    // parameters
    QString fileName;

    // links
    UniSim::Model *plant;

    // housekeeping
    QFile file;

    // methods
    void openFile();
    void write(QString s);
    void writeCR();
    void writeTab();
};

} //namespace


#endif
