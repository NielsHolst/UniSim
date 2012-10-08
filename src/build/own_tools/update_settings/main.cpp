#include <iostream>
#include <QCoreApplication>
#include <usbase/file_location_info.h>
#include "../common/common.h"

using namespace UniSim;
using namespace std;

QDir getModelsDir() {
    QDir dir = getSourceDir();
    dir.cdUp();
    dir.cd("recipes");
    return dir;
}

QDir getApplicationsDir() {
    QDir dir = getSourceDir();
    dir.cd("applications");
    return dir;
}

void cdmk(QDir &dir, QString subDirName) {
    if (!dir.exists(subDirName))
        dir.mkdir(subDirName);
    dir.cd(subDirName);
}

int main(int argc, char *argv[])
{    
    QCoreApplication::setOrganizationName("Aarhus University");
    QCoreApplication::setOrganizationDomain("www.agrsci.dk");
    QCoreApplication::setApplicationName("Universal Simulator");

    QDir dir;

    dir = getPluginsDir();
    FileLocationInfo::setLocation(FileLocationInfo::Plugins, dir);

    dir = getModelsDir();
    FileLocationInfo::setLocation(FileLocationInfo::Models, dir);

    dir = getApplicationsDir();
    cdmk(dir, "output");
    FileLocationInfo::setLocation(FileLocationInfo::Output, dir);

    dir = getApplicationsDir();
    cdmk(dir, "temporary");
    FileLocationInfo::setLocation(FileLocationInfo::Temporary, dir);

    return 0;
}
