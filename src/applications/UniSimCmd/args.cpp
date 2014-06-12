#include <string>
#include <tclap/CmdLine.h>
#include <usbase/exception.h>
#include "args.h"

using namespace std;
using namespace TCLAP;

inline QDir getDir(string s) {
    QDir dir;
    dir.setPath(s.c_str());
    return dir;
}

void Args::parse(int argc, char *argv[]) {
    try {
        CmdLine cmd("Universal Simulator command line tool.\nHome page: www.ecolmod.org", ' ', "1.47");
        ValueArg<string> inputFileNameArg("i","inputfile","Name of XML input file",true,"","string", cmd);
        ValueArg<string> outputFileNameArg("o","outputfile","Name of tab-separated output file",true,"","string", cmd);
        ValueArg<string> inputDirArg("I","inputdir","Name of directory containing XML input file",false,"","string", cmd);
        ValueArg<string> outputDirArg("O","outputdir","Name of directory for tab-separated output file",false,"","string", cmd);
        ValueArg<string> pluginsDirArg("P","pluginsdir","Name of directory with plugin DLLs",false,"","string", cmd);
        ValueArg<string> tempDirArg("T","tempdir","Name of directory for temporary files",false,"","string", cmd);
        cmd.parse(argc,argv);
        inputFileName = inputFileNameArg.getValue().c_str();
        outputFileName = outputFileNameArg.getValue().c_str();
        inputDir = getDir( inputDirArg.getValue() );
        outputDir = getDir( outputDirArg.getValue() );
        pluginsDir = getDir( pluginsDirArg.getValue() );
        tempDir = getDir( tempDirArg.getValue() );
        inputFilePath = inputDir.absoluteFilePath(inputFileName);
        outputFilePath = outputDir.absoluteFilePath(outputFileName);
    }
    catch (ArgException &e) {
        QString msg{"Error in command line arguments:\n"};
        throw UniSim::Exception(msg + e.error().c_str());
    }
}
