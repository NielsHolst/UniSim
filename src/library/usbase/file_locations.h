/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UniSim_FILE_LOCATIONS_H
#define UniSim_FILE_LOCATIONS_H

#include <QDir>
#include <QMap>
#include <QObject>
#include "file_location_info.h"

namespace UniSim {

class FileLocations : public QObject
{
    Q_OBJECT
public:
    FileLocations();
    static QString id();
    virtual ~FileLocations() { }
    static QDir location(FileLocationInfo::FileType fileType);
    static QDir possibleLocation(FileLocationInfo::FileType fileType);
    static bool lookup(FileLocationInfo::FileType fileType, QString message = QString());
private:
    virtual QDir locationImpl(FileLocationInfo::FileType fileType) = 0;
    virtual bool lookupImpl(FileLocationInfo::FileType fileType, QString message = QString()) = 0;

    static FileLocations* me();
    static FileLocations *_me;
};

}
#endif
