#include "../common/common.h"

void writeDir(QString name, QDir dir) {
    QString statement = name + " = \"" + dir.absolutePath() + "\"\n";
    write(statement);
}


int main(int, char **)
{
    QString filePath = getSourceDir().absolutePath() + "/unisim_root.pri";
    openFile(filePath);

    writeTimeStamp();
    writeDir("UNISIM_ROOT", getUniSimDir());

    closeFile();
    return 0;
}
