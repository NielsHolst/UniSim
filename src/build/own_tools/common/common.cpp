#include <iostream>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QStringList>


namespace {

    const char *UNISIM_REL_PATH = "../../..";
    QFile f;

    bool hasProject(QString folderPath, QString folderName) {
        return QFileInfo(folderPath + "/" + folderName + ".pro").exists();
    }
}


void openFile(QString filePath) {
    f.setFileName(filePath);
    f.open(QFile::WriteOnly);
}

void closeFile() {
    f.close();
}

QDir getUniSimDir() {
	return QDir(UNISIM_REL_PATH);
}

QDir getSourceDir() {
    QDir dir = getUniSimDir();
    bool ok = dir.cd("src");
    if (!ok)
        std::cerr << "Cannot find source directory";
    return dir;
}

QDir getPluginsDir() {
    QDir dir = getUniSimDir();
    bool ok = dir.cd("src/plugins");
    if (!ok)
        std::cerr << "Cannot find plugins directory";
    return dir;
}
 
QStringList getPluginFolders() {
	QStringList pluginFolders;
    QFileInfoList dirs = getPluginsDir().entryInfoList(QDir::AllDirs);
    for (int i = 0; i < dirs.size(); ++i) {
        QString folderPath = dirs[i].absoluteFilePath();
        QString folderName = dirs[i].fileName();
        if (hasProject(folderPath, folderName))
            pluginFolders.append(folderName);
    }
	return pluginFolders;
}

void writeTimeStamp(QString lead) {
    QString timeStamp = lead + "Generated on " +
                        QDateTime::currentDateTime().toString("d MMM yy h:mm") + "\n";
    f.write(qPrintable(timeStamp));
}

void write(QString s) {
    f.write(qPrintable(s));
}
