/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_ENGINE_FILE_H
#define UNISIM_ENGINE_FILE_H

#include <QFile>

namespace UniSim {

    class File {
    public:
        enum Mode {Input, Output, TemporaryInput, TemporaryOutput, EngineTest};
        File(Mode mode);
        void open(QString fileName);
        void close();
        static QString filePath(QString fileName, Mode mode);
        QFile& file();
    private:
        QFile _file;
        Mode _mode;
        bool isInput() const;
    };



} //namespace

#endif
