/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include <usbase/file_location_info.h>
#include "file.h"
#include "simulation.h"

namespace UniSim {

File::File(Mode mode) : _mode(mode) {}

void File::open(QString fileName) {
    QString filePath = File::filePath(fileName, _mode);
    _file.setFileName(filePath);
    QIODevice::OpenMode devMode = QIODevice::Text
                                  | ( isInput()
                                      ? QIODevice::ReadOnly
                                      : QIODevice::WriteOnly );
    if (!_file.open(devMode)) {
        QString msg("Cannot open file '%1' for %2");
        QString direction = isInput() ? "input" : "output";
        throw Exception(msg.arg(filePath).arg(direction));
    }
}

bool File::isInput() const {
    return _mode==Input || _mode==TemporaryInput || _mode==EngineTest;
}

QString File::filePath(QString fileName, Mode mode) {
    QDir dir;
    QString filePath;
    switch(mode) {
    case Input:
        filePath = simulation()->inputFilePath(fileName);
        break;
    case Output:
        dir = FileLocationInfo::getLocation(FileLocationInfo::Output);
        break;
    case TemporaryInput:
    case TemporaryOutput:
        dir = FileLocationInfo::getLocation(FileLocationInfo::Temporary);
        break;
    case EngineTest:
        dir = QDir("../library/usengine/test/input");
        break;
    }
    if (filePath.isEmpty())
        filePath = dir.absoluteFilePath(fileName);
    return filePath;
}

void File::close() {
    _file.close();
}

QFile& File::file() {
    return _file;
}
} // namespace
