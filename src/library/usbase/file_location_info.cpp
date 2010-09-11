/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QSettings>

#include "file_location_info.h"

namespace {
    #ifdef UNISIM_DEVELOPMENT
        const char GROUP[] = "Development file locations";
    #else
        const char GROUP[] = "File locations";
    #endif
}

namespace UniSim{

QMap<FileLocationInfo::FileType, QString>
    FileLocationInfo::labels,
    FileLocationInfo::hints;

QString FileLocationInfo::label(FileType fileType) {
    if (labels.isEmpty())
        setLabels();
    return labels[fileType];
}

void FileLocationInfo::setLabels() {
    labels[Plugins] = "Plugin files";
    labels[Models] = "Model files";
    labels[Prototypes] = "Prototype files";
    labels[Output] = "Output files";
    labels[DotTool] = "Dot tool";
    labels[Temporary] = "Temporary files";
    labels[Weather] = "Weather files";
}

QString FileLocationInfo::hint(FileType fileType) {
    if (hints.isEmpty())
        setHints();
    return hints[fileType];
}

void FileLocationInfo::setHints() {
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

void FileLocationInfo::setLocation(FileType fileType, QDir location) {
    QSettings settings;
    QString key = label(fileType);
    settings.beginGroup(GROUP);
    settings.setValue(key, location.absolutePath());
}

void FileLocationInfo::setLocation(FileType fileType, QString filePath) {
    setLocation(fileType, QFileInfo(filePath).dir());
}

QDir FileLocationInfo::getLocation(FileType fileType) {
    QSettings settings;
    QString key = label(fileType);
    QDir dir;
    settings.beginGroup(GROUP);
    if (settings.contains(key))
        dir = QDir(settings.value(key).toString() );
    return dir;
}

bool FileLocationInfo::contains(FileType fileType) {
    QSettings settings;
    settings.beginGroup(GROUP);
    return settings.contains(label(fileType));
}

}
