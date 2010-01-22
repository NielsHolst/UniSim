/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "file_locations_forgiving.h"

#include <QFileDialog>
#include "file_location_dialog.h"

using UniSim::FileLocations;

FileLocationDialog::FileLocationDialog(FileLocations::FileType fileType)
    : _location()
{
    dialog = new QFileDialog(0, "Select folder for " + FileLocations::label(fileType).toLower());
    dialog->setDirectory(FileLocations::possibleLocation(fileType));
    dialog->setFileMode(QFileDialog::Directory);
}

FileLocationDialog::~FileLocationDialog() {
    delete dialog;
}

bool FileLocationDialog::exec() {
    QStringList fileNames;
    QDir dir;
    if (dialog->exec()) {
        fileNames = dialog->selectedFiles();
        if (fileNames.size() > 0) {
            _location = QDir(fileNames[0]);
            return true;
        }
    }
    return false;
}

QDir FileLocationDialog::location() const {
    return _location;
}

