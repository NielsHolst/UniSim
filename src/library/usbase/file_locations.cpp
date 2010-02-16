/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QFileInfo>
#include <QSettings>
#include <usbase/object_pool.h>
#include "file_locations.h"

namespace UniSim{

FileLocations* FileLocations::_me = 0;

#ifdef UNISIM_DEVELOPMENT
    const char GROUP[] = "Development file locations";
#else
    const char GROUP[] = "File locations";
#endif

FileLocations::FileLocations() {
    setLabels();
    setHints();
    setLocations();
    Q_ASSERT(labels.size() == NumLocations);
    Q_ASSERT(hints.size() == NumLocations);
    Q_ASSERT(locations.size() <= NumLocations);
}

FileLocations* FileLocations::me() {
    if (!_me)
        _me = objectPool()->find<FileLocations*>(id());
    return _me;
}

QString FileLocations::id() {
    return "FileLocations";
}

void FileLocations::setLabels() {
    labels[Plugins] = "Plugin files";
    labels[Models] = "Model files";
    labels[Prototypes] = "Prototype files";
    labels[Output] = "Output files";
    labels[DotTool] = "Dot tool";
    labels[Temporary] = "Temporary files";
    labels[Weather] = "Weather files";
}

void FileLocations::setHints() {
    QString beginning, ending = "\n\nYou need to supply the information only this once.";
    hints[Plugins] = beginning +
                    "Universal Simulator needs to know where the plugin files are "
                    "because the plugin files define model behaviour.\n\n"
                    "They are located in a folder called 'plugins'."
                    + ending;
    hints[Models] = "Could not find model folder from your previous session";
    hints[Prototypes] = "Could not find prototype folder from your previous session";
    hints[Output] = beginning +
                    "Universal Simulator needs to know where to put the output files.\n\n"
                    "For this purpose create or select any folder."
                    + ending;
    hints[DotTool] = beginning +
                    "Universal Simulator needs to know where the 'dot' tool is "
                    "because this tool is used to draw model diagrams.\n\n"
                    "The 'dot' tool is likely located in a folder called 'graphvizx.xx/bin', "
                    "on MS Windows found in e.g. the 'Program Files' folder."
                    + ending;
    hints[Temporary] = beginning +
                    "Universal Simulator needs to store temporary files somewhere.\n\n"
                    "For this purpose create or select any folder."
                    + ending;
    hints[Weather] = beginning +
                    "Universal Simulator needs to know where you have put the weather files.\n\n"
                    + ending;
}

void FileLocations::setLocations() {
    QSettings settings;
    settings.beginGroup(GROUP);
    for (int i = 0; i < int(NumLocations); i++) {
        FileType fileType = FileType(i);
        if (settings.contains(labels[fileType]))
            locations[fileType] = QDir(settings.value(labels[fileType]).toString() );
    }
}

bool FileLocations::contain(FileType fileType) {
    return me()->locations.contains(fileType);
}

QString FileLocations::label(FileType fileType) {
    return me()->labels[fileType];
}

QString FileLocations::hint(FileType fileType) {
    return me()->hints[fileType];
}

QDir FileLocations::location(FileType fileType) {
    return me()->locationImpl(fileType);
}

bool FileLocations::lookup(FileType fileType, QString message) {
    return me()->lookupImpl(fileType, message);
}

QDir FileLocations::possibleLocation(FileType fileType) {
    if (me()->contain(fileType)) {
        QDir dir = me()->locations[fileType];
        if (dir.exists()) return dir;
    };
    return QDir();
}

void FileLocations::setLocation(FileType fileType, QDir location) {
	QSettings settings;
    settings.beginGroup(GROUP);
    settings.setValue(me()->labels[fileType], location.absolutePath());
	
    me()->locations[fileType] = location;
}

void FileLocations::setLocation(FileType fileType, QString filePath) {
    setLocation(fileType, QFileInfo(filePath).dir());
}

}
