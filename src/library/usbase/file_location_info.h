/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_FILE_LOCATION_INFO
#define UNISIM_FILE_LOCATION_INFO

#include <QDir>
#include <QMap>

namespace UniSim {

class FileLocationInfo
{

public:
    enum FileType {Plugins, DotTool, Models, Output, Prototypes, Temporary, NumLocations};
    static void initialize();
    static QString label(FileType fileType);
    static QString hint(FileType fileType);
    static void setLocation(FileType fileType, QDir location);
    static void setLocation(FileType fileType, QString filePath);
    static QDir getLocation(FileType fileType);
    static bool contains(FileType fileType);
private:
    static QMap<FileType, QString> labels, hints;

    static void setLabels();
    static void setHints();

    static QDir getLocation(FileType fileType, QString group);
    static bool contains(FileType fileType, QString group);


    static QString group();
    static QString otherGroup();
};

}
#endif
