#ifndef ARGS_H
#define ARGS_H
#include <QDir>

struct Args {
    QString inputFileName, outputFileName, inputFilePath, outputFilePath;
    QDir inputDir, outputDir, pluginsDir, tempDir;
    void parse(int argc, char *argv[]);
} ;

#endif // ARGS_H
