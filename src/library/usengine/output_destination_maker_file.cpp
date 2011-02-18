/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QDir>
#include <usbase/file_locations.h>
#include "output.h"
#include "output_destination_file.h"
#include "output_destination_maker_file.h"

namespace UniSim{
	
OutputDestinationMakerFile::OutputDestinationMakerFile(QObject *parent)
	: OutputDestinationMaker(parent)
{	
}

void OutputDestinationMakerFile::createDestination(Output *output) {
	QDir path = FileLocations::location(FileLocationInfo::Output);
    QString filePath = path.absolutePath() + "/" + output->title() + ".txt";

	OutputDestination *destination = new OutputDestinationFile(filePath, output);
	
	output->connectVariables(destination);
}

}
