/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "debug_output.h"
#include "file_locations.h"

namespace UniSim{
	
DebugOutput* DebugOutput::_debugOutput = 0;

DebugOutput::DebugOutput()
    : isOpen(false)
{
    stream.setString(&buffer);
}

QString DebugOutput::id() {
    return "DebugOutput";
}

void DebugOutput::write(QString s) {
    if (!isOpen)
        open();
    file.write(qPrintable(s));
}

void DebugOutput::open() {
    QString path = FileLocations::location(FileLocations::Output).absolutePath();
    QString filePath = path + "/debug_output.prn";
    file.setFileName(filePath);
    if (!file.open(QIODevice::Text | QIODevice::WriteOnly))
        throw Exception("Could not open debug output file:\n'" + filePath + "'");
    isOpen = true;
}

void DebugOutput::writeBuffer() {
    write(buffer);
    buffer.clear();
}

void DebugOutput::write(const char *s) {
    write(QString(s));
}

void DebugOutput::close() {
    file.close();
}

} //namespace

