/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_OUTPUT_PARAMETER_TABLE
#define UNISIM_OUTPUT_PARAMETER_TABLE

#include <QFile>
#include <QList>
#include <usbase/output.h>

namespace UniSim{

class OutputVariable;

class OutputParameterTable : public Output
{
	Q_OBJECT
public:
    OutputParameterTable(Identifier name, QObject *parent=0);
    // standard methods
    void cleanup();
	
private:
    // parameters
    QString fileName;

    // housekeeping
    QFile file;

    // methods
    typedef QList<OutputVariable*> Variables;
    void openFile();
    QString ammendedFileName(QString fileName, int number);
    int checkDataSize(const Variables &variables, int dataSize = 0) const;
    void writeLabels(const Variables &variables);
    void writeData(const Variables &variables, int dataIx);
    void writeCR();
    void writeTab();
    void writeParameters();
};

} //namespace


#endif
