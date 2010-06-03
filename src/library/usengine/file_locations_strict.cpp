/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "file_locations_strict.h"

#include <iostream>

namespace UniSim{

FileLocationsStrict::FileLocationsStrict() : FileLocations() {
}

QDir FileLocationsStrict::locationImpl(FileType fileType) {
    Q_ASSERT_X(contain(fileType),
               "FileLocationsStrict::locationImpl",
               qPrintable("File type missing: " + label(fileType)));
    return locations[fileType];
}

bool FileLocationsStrict::lookupImpl(FileType fileType, QString message) {
    QString msg = message.isEmpty() ?
                  "Error when looking up file type: " + label(fileType) :
                  message;
    Q_ASSERT_X(false,
               "FileLocationsStrict::lookupImpl",
               qPrintable(msg));
    return false;
}

} //namespace
