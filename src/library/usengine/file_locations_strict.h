/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UniSim_FILE_LOCATIONS_STRICT_H
#define UniSim_FILE_LOCATIONS_STRICT_H

#include <QObject>
#include <usbase/file_locations.h>

namespace UniSim {

class FileLocationsStrict : public FileLocations {
public:
    FileLocationsStrict();
private:
    QDir locationImpl(FileLocationInfo::FileType fileType);
    bool lookupImpl(FileLocationInfo::FileType fileType, QString message);
};

}
#endif
