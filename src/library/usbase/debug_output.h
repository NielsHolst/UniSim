/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_DEBUG_OUTPUT_H
#define UNISIM_DEBUG_OUTPUT_H

#include <QFile>
#include <QObject>
#include <QTextStream>
#include <usbase/object_pool.h>

namespace UniSim{
	
class DebugOutput : public QObject
{
    Q_OBJECT

public:
    DebugOutput();
    static QString id();
    void writeBuffer();
    void write(QString s);
    void write(const char *s);
    void close();
private:
    void open();
    // singleton
    static DebugOutput* _debugOutput;
    friend DebugOutput* debugOutput();
    friend QTextStream& debugStream();
    // data
    bool isOpen;
    QFile file;
    QString buffer;
    QTextStream stream;
};

inline DebugOutput* debugOutput()
{
    if (!DebugOutput::_debugOutput) {
        DebugOutput::_debugOutput =
            objectPool()->find<DebugOutput*>(DebugOutput::id());
    }
    return DebugOutput::_debugOutput;
}

inline QTextStream& debugStream() {
    return debugOutput()->stream;
}

} //namespace

#endif
