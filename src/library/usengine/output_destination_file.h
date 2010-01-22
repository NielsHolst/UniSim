/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UniSim_OUTPUT_DESTINATION_FILE_H
#define UniSim_OUTPUT_DESTINATION_FILE_H

#include "output_destination.h"

class QTextStream;

namespace UniSim{

class Output;
class XYStateVariables;

class OutputDestinationFile : public OutputDestination
{
	Q_OBJECT
public:
    OutputDestinationFile(QString filePath, Output *parent = 0);
       
private:
    void showTitle();
    void showLabels();
    void showData();

    void writeLabels(QTextStream &text, const XYStateVariables *xy);

	QString _filePath;
	static int _count;
};

}
#endif
