/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QFileDialog>
#include "file_location_dialog.h"
#include "main_window.h"

using namespace UniSim;

FileLocationDialog::FileLocationDialog(FileLocationInfo::FileType fileType)
    : title("Select folder for " + FileLocationInfo::label(fileType).toLower()),
      folder(FileLocations::possibleLocation(fileType).absolutePath()),
      _location()
{
}

bool FileLocationDialog::exec() {
    QFileDialog dialog(mainWindow(), title, folder);
//    QFileDialog dialog(0, title, folder);
//    dialog.setFileMode(QFileDialog::DirectoryOnly);
    if (dialog.exec()) {
        QStringList fileNames = dialog.selectedFiles();
        if (fileNames.size() > 0) {
            QDir dir = QFileInfo(fileNames[0]).absoluteDir();
            _location = dir.absolutePath();
            return true;
        }
    }
    return false;
}

QDir FileLocationDialog::location() const {
    return _location;
}

