/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QFileInfo>
#include <QSettings>
#include <usbase/object_pool.h>
#include "file_locations.h"

namespace UniSim{

FileLocations* FileLocations::_me = 0;

FileLocations::FileLocations() {
    FileLocationInfo::initialize();
}

FileLocations* FileLocations::me() {
    if (!_me)
        _me = objectPool()->find<FileLocations*>(id());
    return _me;
}

QString FileLocations::id() {
    return "FileLocations";
}

QDir FileLocations::location(FileLocationInfo::FileType fileType) {
    return me()->locationImpl(fileType);
}

QDir FileLocations::possibleLocation(FileLocationInfo::FileType fileType) {
    return FileLocationInfo::getLocation(fileType);
}

bool FileLocations::lookup(FileLocationInfo::FileType fileType, QString message) {
    return me()->lookupImpl(fileType, message);
}
}
