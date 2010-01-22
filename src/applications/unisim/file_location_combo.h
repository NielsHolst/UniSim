/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef FILE_LOCATION_COMBO_H
#define FILE_LOCATION_COMBO_H
#include <QComboBox>
#include <usbase/file_locations.h>

class FileLocationCombo : public QComboBox
{
	Q_OBJECT
public:
    FileLocationCombo(UniSim::FileLocations::FileType fileType, QWidget *parent = 0);
private slots:
    void doBrowse(int index);
private:
    UniSim::FileLocations::FileType fileType;
    bool isBrowsing;
};


#endif
