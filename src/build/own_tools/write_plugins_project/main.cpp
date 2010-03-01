#include "../common/common.h"

int main(int argc, char *argv[])
{
    QStringList pluginFolders = getPluginFolders();

    QString filePath = getPluginsDir().absolutePath() + "/plugins.pro";
    openFile(filePath);

    writeTimeStamp();
    write("TEMPLATE = subdirs\n");

    if (!pluginFolders.isEmpty())
        write("SUBDIRS =");
    for (int i = 0; i < pluginFolders.size(); ++i)
        write(" \\\n\t" + pluginFolders[i]);
    write("\n");

    closeFile();
    return 0;
}
