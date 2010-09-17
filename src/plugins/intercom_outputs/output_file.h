/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_OUTPUT_TABLE
#define UNISIM_OUTPUT_TABLE

#include <QFile>
#include <QList>
#include <usbase/output.h>

namespace UniSim{

class OutputFile : public Output
{
	Q_OBJECT
public:
    OutputFile(Identifier name, QObject *parent=0);
    // standard methods
    void cleanup();
	
private:
    // parameters
    QString fileName;

    // housekeeping
    QFile file;

    // methods
    void openFile();
    write(QString s);
    void writeCR();
    void writeTab();
};

} //namespace


#endif
