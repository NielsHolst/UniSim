#include "../common/common.h"

void turnSlashes(QString &path) {
    path = path.replace("/", "\\");
}

QString cleanCmd(QDir dir, QString folderName) {
    QString folderPath = dir.absolutePath() + "/" + folderName;
    turnSlashes(folderPath);
    return "call clean_folder " + folderPath + "\n";
}

QString delCmd(QDir dir, QString fileName) {
    QString filePath = dir.absolutePath() + "/" + fileName;
    turnSlashes(filePath);
    return "if exist " + filePath +" del " + filePath + " /Q\n";
}

int main(int argc, char *argv[])
{
    QString filePath = "../win/clean_ephemerals.bat";
    openFile(filePath);

    writeTimeStamp("@rem ");
    write("@echo off\n");

    write(cleanCmd(getSourceDir(), "build/own_tools/update_settings"));
    write(cleanCmd(getSourceDir(), "build/own_tools/write_clean_ephemerals"));
    write(cleanCmd(getSourceDir(), "build/own_tools/write_plugins_project"));
    write(cleanCmd(getSourceDir(), "build/own_tools/write_unisim_root"));


    write(cleanCmd(getSourceDir(), "applications/unisim"));
    write(cleanCmd(getSourceDir(), "applications/unisim/test"));

    QStringList pluginFolders = getPluginFolders();
    for (int i = 0; i < pluginFolders.size(); ++i)
        write(cleanCmd(getPluginsDir(), pluginFolders[i]));

    write(delCmd(getSourceDir(), "makefile.*"));
    write(delCmd(getPluginsDir(), "makefile.*"));

    write("@echo on\n");
    write("@echo *\n");
    write("@echo * UniSim project cleaned for all intermediate files *\n");
    write("@echo *\n");

    closeFile();
    return 0;
}
