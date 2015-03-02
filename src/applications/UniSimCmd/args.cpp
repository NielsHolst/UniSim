#include <string>
#include <QFileInfo>
#include <tclap/CmdLine.h>
#include <usbase/exception.h>
#include "args.h"

using namespace std;
using namespace TCLAP;

inline QString filePath(QString dirName, QString fileName) {
    QDir home;
    QString result;
    if (dirName.isEmpty()) {
        if (QDir::isAbsolutePath(fileName))
            result = fileName;
        else
            result = home.absoluteFilePath(fileName);
    }
    else {
        if (QDir::isAbsolutePath(fileName)) {
            QString msg{"Directory argument (%1) cannot be given together with an absolute file name (%2)"};
            throw UniSim::Exception(msg.arg(dirName).arg(fileName));
        }
        else
            result = QDir(dirName).absoluteFilePath(fileName);
    }
    return result;
}

inline QFileInfo info(QString dirName, QString fileName = "") {
    QDir home;
    QFileInfo result;
    enum {Empty, Relative, Absolute} dirType, fileType;

    if (QDir::isAbsolutePath(fileName))
        fileType = Absolute;
    else
        fileType = Relative;

    if (dirName.isEmpty())
        dirType = Empty;
    else if (QDir::isAbsolutePath(dirName))
        dirType = Absolute;
    else
        dirType = Relative;

    QString s;
    if (fileType == Absolute)
        result = QFileInfo(fileName);
    else switch (dirType) {
        case Empty:
            s = home.absolutePath() + "/" + fileName;
            result = QFileInfo(s); break;
        case Relative:
            s = home.absolutePath() + "/" + dirName + "/" + fileName;
            result = QFileInfo(s); break;
        case Absolute:
            s = dirName + "/" + fileName;
            result = QFileInfo(s); break;
    }
    return result;
}

void Args::parse(int argc, char *argv[]) {
    try {
        CmdLine cmd("Universal Simulator command line tool.\nHome page: www.ecolmod.org", ' ', "1.48");

        ValueArg<string> pluginsDirArg("P","pluginsdir",
            "Name of directory with plugin DLLs, defaults to  '../bin/plugins'",
            false,"../bin/plugins","string", cmd);

        ValueArg<string> tempDirArg("T","tempdir",
            "Name of directory for temporary files, defaults to the current user temp directory",
            false, QDir::tempPath().toStdString(),"string", cmd);

        ValueArg<string> outputDirArg("O","outputdir",
            "Name of directory for tab-separated output file, will be prepended to the outputfile argument. Defaults to the installation directory",
            false,"","string", cmd);

        ValueArg<string> inputDirArg("I","inputdir",
            "Name of directory containing XML input file, will be prepended to the inputfile argument. Defaults to the installation directory",
            false,"","string", cmd);

        ValueArg<string> outputFileNameArg("o","outputfile",
            "Name of tab-separated output file, with or with path",
            true,"","string", cmd);

        ValueArg<string> inputFileNameArg("i","inputfile",
            "Name of XML input file, with or with path",
            true,"","string", cmd);

        cmd.parse(argc,argv);

        QFileInfo
            inputInfo = info(inputDirArg.getValue().c_str(), inputFileNameArg.getValue().c_str()),
            outputInfo = info(outputDirArg.getValue().c_str(), outputFileNameArg.getValue().c_str()),
            pluginsInfo = info(pluginsDirArg.getValue().c_str()),
            tempInfo = info(tempDirArg.getValue().c_str());

        inputDir = inputInfo.absoluteDir();
        outputDir = outputInfo.absoluteDir();
        pluginsDir = pluginsInfo.absoluteDir();
        tempDir = tempInfo.absoluteDir();
        inputFilePath = inputInfo.absoluteFilePath();
        outputFilePath = outputInfo.absoluteFilePath();
        outputFileName = outputInfo.fileName();
    }
    catch (ArgException &e) {
        QString msg{"Error in command line arguments:\n"};
        throw UniSim::Exception(msg + e.error().c_str());
    }
}
