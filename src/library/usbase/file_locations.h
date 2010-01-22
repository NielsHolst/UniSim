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

namespace UniSim {

class FileLocations : public QObject
{
    Q_OBJECT

public:
    enum FileType {Plugins, DotTool, Models, Output, Prototypes, Temporary, NumLocations};
    FileLocations();
    static QString id();
    virtual ~FileLocations() { }
    static bool contain(FileType fileType);
    static QString label(FileType fileType);
    static QString hint(FileType fileType);
    static QDir location(FileType fileType);
    static QDir possibleLocation(FileType fileType);
    static void setLocation(FileType fileType, QDir location);
    static void setLocation(FileType fileType, QString filePath);
protected:
    QMap<FileType, QDir> locations;
private:
    virtual QDir locationImpl(FileType fileType) = 0;
    static FileLocations* me();

    void setLabels();
    void setHints();
    void setLocations();

    static FileLocations *_me;
    QMap<FileType, QString> labels, hints;
};

}
#endif
