/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QSettings>
#include "file_location_info.h"
#include "version.h"

const char DEV_GROUP[] = "Development file locations";
const char USER_GROUP[] = "File locations";

namespace UniSim {

// static data
QMap<FileLocationInfo::FileType, QString>
    FileLocationInfo::labels,
    FileLocationInfo::hints;

// member functions


void FileLocationInfo::initialize() {
    if (isDeveloperVersion() && !contains(DotTool) && contains(DotTool, otherGroup())) {
        QDir dir = getLocation(DotTool, otherGroup());
        setLocation(DotTool, dir);
    }
}

QString FileLocationInfo::group() {
    return isDeveloperVersion() ? DEV_GROUP : USER_GROUP;
}

QString FileLocationInfo::otherGroup() {
    return isDeveloperVersion() ? USER_GROUP : DEV_GROUP;
}

QString FileLocationInfo::label(FileType fileType) {
    if (labels.isEmpty())
        setLabels();
    return labels[fileType];
}

void FileLocationInfo::setLabels() {
    labels[Plugins] = "Plugin files";
    labels[Models] = "Model files";
    labels[Output] = "Output files";
    labels[DotTool] = "Dot tool";
    labels[Temporary] = "Temporary files";
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
}

void FileLocationInfo::setLocation(FileType fileType, QDir location) {
    QSettings settings;
    QString key = label(fileType);
    settings.beginGroup(group());
    settings.setValue(key, location.absolutePath());
}

void FileLocationInfo::setLocation(FileType fileType, QString filePath) {
    setLocation(fileType, QFileInfo(filePath).dir());
}

QDir FileLocationInfo::getLocation(FileType fileType) {
    return getLocation(fileType, group());
}

QDir FileLocationInfo::getLocation(FileType fileType, QString group) {
    QSettings settings;
    QString key = label(fileType);
    QDir dir;
    settings.beginGroup(group);
    if (settings.contains(key))
        dir = QDir(settings.value(key).toString() );
    return dir;
}

bool FileLocationInfo::contains(FileType fileType) {
    return contains(fileType, group());
}

bool FileLocationInfo::contains(FileType fileType, QString group) {
    QSettings settings;
    settings.beginGroup(group);
    return settings.contains(label(fileType));
}

}
