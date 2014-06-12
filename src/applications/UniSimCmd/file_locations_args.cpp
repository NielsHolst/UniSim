/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "usbase/exception.h"
#include "file_locations_args.h"

using namespace UniSim;

FileLocationsArgs::FileLocationsArgs(const Args &args_)
    : FileLocations(), args(args_) {
}

QDir FileLocationsArgs::locationImpl(FileLocationInfo::FileType fileType) {
    QDir dir;
    QString msg;
    switch (fileType) {
    case FileLocationInfo::Plugins:
        dir = args.pluginsDir;
        break;
    case FileLocationInfo::DotTool:
        msg = "'dot' tool should not be used";
        break;
    case FileLocationInfo::Models:
        dir = args.inputDir;
        break;
    case FileLocationInfo::Output:
        dir = args.outputDir;
        break;
    case FileLocationInfo::Temporary:
        dir = args.tempDir;
        break;
    default:
        msg = "Unknown file location code: %1";
        msg = msg.arg(int(fileType));
    }
    if (!msg.isEmpty())
        throw Exception("Program error. " + msg);
    return dir;
}

bool FileLocationsArgs::lookupImpl(FileLocationInfo::FileType, QString ) {
    return false;
}

