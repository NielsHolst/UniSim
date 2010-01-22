/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <QFile>
#include <QList>
#include <QTextStream>
#include <usbase/exception.h>
#include "output.h"
#include "output_destination_file.h"
#include "xy_state_variables.h"

namespace UniSim{

int OutputDestinationFile::_count = 0;
	
OutputDestinationFile::OutputDestinationFile(QString filePath, Output *parent)
    : OutputDestination("output_file_"+QString::number(++_count), parent),
    _filePath(filePath)
{
}

void OutputDestinationFile::showTitle() {
}

void OutputDestinationFile::showLabels() {
}

void OutputDestinationFile::showData() {
	QFile file(_filePath);
	bool fileOk = file.open(QIODevice::WriteOnly | QIODevice::Text);
	if (!fileOk)
		throw Exception("Cannot write to file: " + _filePath);

    for (QList<XYStateVariables*>::const_iterator xyi = _xyList->begin(); xyi != _xyList->end(); ++xyi) {
        XYStateVariables *xy = *xyi;
        const XYStateVariables::Variable &x(xy->xVariables()[0]);

        QString s;
        QTextStream text(&s);
        if (xyi != _xyList->begin()) file.write("\n");
        writeLabels(text, xy);

        for (int i = 0; i < xy->yVariables().size(); ++i) {
            const XYStateVariables::Variable &y(xy->yVariables()[i]);

            Q_ASSERT(x.data.size() == y.data.size());
            if (i > 0) file.write("\n");
            for (int j = 0; j < x.data.size(); ++j) {
                text << x.data[j]<< "\t" << y.data[j] << "\n";
            }
		}
		file.write(qPrintable(s));
	}
	file.close();
}

void OutputDestinationFile::writeLabels(QTextStream &text, const XYStateVariables *xy) {
    Q_ASSERT(xy->xVariables().size() == 1);
    text << xy->xVariables()[0].label;

    QStringList yLabels = xy->uniqueYLabels();
    for (int i = 0; i < yLabels.size(); ++i)
        text << "\t" << yLabels[i];
    text << "\n";
}

}
