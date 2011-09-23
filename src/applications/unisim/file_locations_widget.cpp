/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QtGui>
#include <usbase/file_locations.h>
#include "file_location_combo.h"
#include "file_location_dialog.h"
#include "file_locations_widget.h"
#include "sub_window.h"

using namespace UniSim;

FileLocationsWidget::FileLocationsWidget(QWidget *parent)
    : QWidget(parent), parentWidget(parent)
{
    setParent(parent);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addLayout(createEntries());
    layout->addLayout(createButtons());
    connect(closeButton, SIGNAL(clicked()), this, SLOT(doClose()));
}

void FileLocationsWidget::doClose() {
    parentWidget->close();
}

QLayout* FileLocationsWidget::createEntries() {
    QGridLayout *layout = new QGridLayout;
    for (int row = 0; row < FileLocationInfo::NumLocations; ++row) {
        FileLocationInfo::FileType fileType = FileLocationInfo::FileType(row);
        layout->addWidget(new QLabel(FileLocationInfo::label(fileType), this), row, 0);

        FileLocationCombo *combo;
        layout->addWidget(combo = new FileLocationCombo(fileType, this), row, 1);
    }
    return layout;
}

QLayout* FileLocationsWidget::createButtons() {
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addStretch();
    layout->addWidget(closeButton = new QPushButton("Close", this));
    /* closeButton->setDefault(true);
    ** This does not work. The keyboard event does not propagate through.
    */
    return layout;
}

