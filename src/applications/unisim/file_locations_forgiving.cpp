/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "file_location_dialog.h"
#include "file_locations_forgiving.h"

#include <QFileDialog>
#include <QMessageBox>


using namespace UniSim;

FileLocationsForgiving::FileLocationsForgiving()
    : FileLocations() {
}

QDir FileLocationsForgiving::locationImpl(FileLocationInfo::FileType fileType) {
    if (FileLocationInfo::contains(fileType)) {
        QDir dir = FileLocationInfo::getLocation(fileType);
        if (dir.exists()) return dir;
    }
    else if (lookupImpl(fileType))
        return locationImpl(fileType);
    return QDir();
}

bool FileLocationsForgiving::lookupImpl(FileLocationInfo::FileType fileType, QString message) {
    QString msg = message + (message.isEmpty() ? "" : "\n\n") +
                  FileLocationInfo::hint(fileType);
    QMessageBox::information(0, "Guidance", msg);

    FileLocationDialog dialog(fileType);
    if (dialog.exec()) {
        FileLocationInfo::setLocation(fileType, dialog.location());
        return true;
    }
    return false;
}

