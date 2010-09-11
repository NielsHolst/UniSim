/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef FILE_LOCATION_DIALOG_H
#define FILE_LOCATION_DIALOG_H

#include <QDir>
#include <usbase/file_locations.h>

class QFileDialog;

class FileLocationDialog
{
public:
    FileLocationDialog(UniSim::FileLocationInfo::FileType fileType);
    ~FileLocationDialog();
    bool exec();
    QDir location() const;
private:
    QFileDialog *dialog;
    QDir _location;
};

#endif
