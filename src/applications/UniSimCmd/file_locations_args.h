/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef FILE_LOCATIONS_ARGS_H
#define FILE_LOCATIONS_ARGS_H

#include <usbase/file_location_info.h>
#include <usbase/file_locations.h>
#include "args.h"

class FileLocationsArgs : public UniSim::FileLocations {
public:
    FileLocationsArgs(const Args &args);
private:
    QDir locationImpl(UniSim::FileLocationInfo::FileType fileType);
    bool lookupImpl(UniSim::FileLocationInfo::FileType fileType, QString message = QString());
    Args args;
};

#endif
