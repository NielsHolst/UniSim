#include <QDir>
#include <QStringList>

void openFile(QString filePath);
void closeFile();

QDir getUniSimDir();
QDir getSourceDir();
QDir getPluginsDir();
QStringList getPluginFolders();

void writeTimeStamp(QString lead = "# ");
void write(QString s);


