#include <QtGlobal>
#include "../common/common.h"

void writeDir(QString name, QDir dir) {
    QString statement = name + " = \"" + dir.absolutePath() + "\"\n";
    write(statement);
}


int main(int, char **)
{
    QString path = QString(qgetenv("PATH")).toLower();
    int version =  path.contains("qtsdk") ? 4 : 5;

    QString filePath = getSourceDir().absolutePath() + "/build/win/copy_qwt.bat";
    openFile(filePath);

    writeTimeStamp("@rem ");
    write(QString("copy ..\\..\\vendor\\qwt_qt%1\\lib\\*.dll").arg(version));

    closeFile();
    return 0;
}
