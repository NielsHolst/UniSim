/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_OUTPUT_TABLE_BASE
#define UNISIM_OUTPUT_TABLE_BASE

#include <QFile>
#include <QList>
#include <usbase/file_location_info.h>
#include <usbase/output.h>

namespace UniSim{

class OutputTableBase : public Output
{
	//Q_OBJECT
public:
    OutputTableBase(Identifier name, QObject *parent=0);
    // special methods
    int numXTraces() const;
protected:
    void openFile(QString appendix = QString());
    void writeString(QString s);
    void closeFile();
    int traceSize();
private:
    // parameters
    QString fileName;

    // housekeeping
    QFile file;
    QStringList outputFilePaths;

    // methods
    QString insertAppendix(QString fileName, QString appendix);
    QString insertNumber(QString fileName, int number);
};

} //namespace


#endif
