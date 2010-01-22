/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QtGui>
#include <usbase/file_locations.h>
#include "file_location_combo.h"
#include "file_location_dialog.h"

using namespace UniSim;

FileLocationCombo::FileLocationCombo(FileLocations::FileType fType, QWidget *parent)
    :  QComboBox(parent), fileType(fType), isBrowsing(false)
{
    QString location = FileLocations::possibleLocation(fileType).absolutePath();
    addItem(location);
    addItem("Browse...");
    setCurrentIndex(0);
    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(doBrowse(int)));
}

void FileLocationCombo::doBrowse(int index) {
    if (isBrowsing) return;
    isBrowsing = true;
    setEnabled(false);
    FileLocationDialog dialog(fileType);
    if (dialog.exec()) {
        removeItem(0);
        insertItem(0, dialog.location().absolutePath());
        FileLocations::setLocation(fileType, dialog.location());
    }
    setCurrentIndex(0);
    setEnabled(true);
    isBrowsing = false;
}
