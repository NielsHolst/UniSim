#include <iostream>
#include <QCoreApplication>
#include <usbase/file_location_info.h>
#include "../common/common.h"

using namespace UniSim;
using namespace std;

QDir getXmlDir() {
    QDir dir = getSourceDir();
    dir.cdUp();
    dir.cd("xml");
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

    dir = getXmlDir();
    dir.cd("models");
    FileLocationInfo::setLocation(FileLocationInfo::Models, dir);
    //cout << qPrintable(dir.absolutePath()) << "\n";
    //getchar();

    dir = getXmlDir();
    dir.cd("prototypes");
    FileLocationInfo::setLocation(FileLocationInfo::Prototypes, dir);

    dir = getApplicationsDir();
    cdmk(dir, "output");
    FileLocationInfo::setLocation(FileLocationInfo::Output, dir);

    dir = getApplicationsDir();
    cdmk(dir, "temporary");
    FileLocationInfo::setLocation(FileLocationInfo::Temporary, dir);

    dir = getUniSimDir();
    dir.cd("weather");
    FileLocationInfo::setLocation(FileLocationInfo::Weather, dir);

    dir = getUniSimDir();
    dir.cd("datasets");
    FileLocationInfo::setLocation(FileLocationInfo::Datasets, dir);

    return 0;
}
