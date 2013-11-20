/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QtGui>
#include <usbase/file_locations.h>
#include "file_location_combo.h"
#include "file_location_dialog.h"

using namespace UniSim;

FileLocationCombo::FileLocationCombo(FileLocationInfo::FileType fType, QWidget *parent)
    :  QComboBox(parent), fileType(fType), isBrowsing(false)
{
    addItem(currentFolder = FileLocations::possibleLocation(fileType).absolutePath());
    addItem("Browse...");
    setCurrentIndex(0);
    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(doBrowse(int)));
}

void FileLocationCombo::doBrowse(int /*index*/) {
    if (isBrowsing)
        return;
    isBrowsing = true;
    FileLocationDialog dialog(fileType);
    if (dialog.exec()) {
        removeItem(0);
        insertItem(0, dialog.location().absolutePath());
        FileLocationInfo::setLocation(fileType, dialog.location());
    }

//    QString selectedFolder =
//        QFileDialog::getExistingDirectory(this, "Select a folder");
//    if (!selectedFolder.isEmpty()) {
//        removeItem(0);
//        insertItem(0, selectedFolder);
//        //FileLocationInfo::setLocation(fileType, selectedFolder);
//    }

//    QFileDialog *dialog = new QFileDialog(this, "Select a folder", "C:\\");
//    dialog->setFileMode(QFileDialog::Directory);
//    dialog->setOption(QFileDialog::ShowDirsOnly);
//    if (dialog->exec()) {
//        QString selectedFolder = "TST";
//        removeItem(0);
//        insertItem(0, selectedFolder);

//    }

    QWidget *parentDialog = parentWidget();
    setCurrentIndex(0);
    parentDialog->raise();
    parentDialog->activateWindow();
    isBrowsing = false;
}
