#include <iostream>
#include <QDir>
#include <usbase/exception.h>
#include "../args.h"
#include "test_args.h"

using namespace std;
using namespace UniSim;

const QString homeDir{"C:/data/QDev/UniSim/src/applications"};

void TestArgs::testPluginsDefault() {
    Args args;
    char *s[]  = {"UniSimCmd", "-i", "dummy", "-o", "dummy"};
    args.parse(5, s);
    QCOMPARE(args.pluginsDir.absolutePath(), homeDir+"/plugins");
}

void TestArgs::testPluginsRelative() {
    Args args;
    char *s[]  = {"UniSimCmd", "-P", "local", "-i", "dummy", "-o", "dummy"};
    args.parse(7, s);
    QCOMPARE(args.pluginsDir.absolutePath(), homeDir+"/local");
}

void TestArgs::testPluginsAbsolute() {
    Args args;
    char *s[]  = {"UniSimCmd", "-P", "C:\\abc\\plugins", "-i", "dummy", "-o", "dummy"};
    args.parse(7, s);
    QCOMPARE(args.pluginsDir.absolutePath(), QString("C:/abc/plugins"));
}

void TestArgs::testTempDefault() {
    Args args;
    char *s[]  = {"UniSimCmd", "-i", "dummy", "-o", "dummy"};
    args.parse(5, s);
    QString tempPath = args.tempDir.absolutePath();
    QCOMPARE(tempPath.left(8), QString("C:/Users"));
    QCOMPARE(tempPath.right(4), QString("Temp"));
}

void TestArgs::testTempRelative() {
    Args args;
    char *s[]  = {"UniSimCmd", "-T", "local", "-i", "dummy", "-o", "dummy"};
    args.parse(7, s);
    QCOMPARE(args.tempDir.absolutePath(), homeDir+"/local");
}

void TestArgs::testTempinsAbsolute() {
    Args args;
    char *s[]  = {"UniSimCmd", "-T", "C:\\abc\\temp", "-i", "dummy", "-o", "dummy"};
    args.parse(7, s);
    QCOMPARE(args.tempDir.absolutePath(), QString("C:/abc/temp"));
}

void TestArgs::testInputDirNoneFileBare() {
    Args args;
    char *s[]  = {"UniSimCmd", "-i", "input.xml", "-o", "dummy"};
    args.parse(5, s);
    QCOMPARE(args.inputDir.absolutePath(), homeDir);
    QCOMPARE(args.inputFilePath, homeDir+"/input.xml");
}

void TestArgs::testInputDirNoneFileRelative() {
    Args args;
    char *s[]  = {"UniSimCmd", "-i", "local/input.xml", "-o", "dummy"};
    args.parse(5, s);
    QCOMPARE(args.inputDir.absolutePath(), homeDir+"/local");
    QCOMPARE(args.inputFilePath, homeDir+"/local/input.xml");
}

void TestArgs::testInputDirNoneFileAbsolute() {
    Args args;
    char *s[]  = {"UniSimCmd", "-i", "C:\\abc\\input.xml", "-o", "dummy"};
    args.parse(5, s);
    QCOMPARE(args.inputDir.absolutePath(), QString("C:/abc"));
    QCOMPARE(args.inputFilePath, QString("C:/abc/input.xml"));
}

void TestArgs::testInputDirRelativeFileBare() {
    Args args;
    char *s[]  = {"UniSimCmd", "-i", "input.xml", "-o", "dummy", "-I", "local"};
    args.parse(7, s);
    QCOMPARE(args.inputDir.absolutePath(), homeDir+"/local");
    QCOMPARE(args.inputFilePath, homeDir+"/local/input.xml");
}

void TestArgs::testInputDirRelativeFileRelative() {
    Args args;
    char *s[]  = {"UniSimCmd", "-i", "here/input.xml", "-o", "dummy", "-I", "local"};
    args.parse(7, s);
    QCOMPARE(args.inputDir.absolutePath(), homeDir+"/local/here");
    QCOMPARE(args.inputFilePath, homeDir+"/local/here/input.xml");
}

void TestArgs::testInputDirRelativeFileAbsolute() {
    Args args;
    char *s[]  = {"UniSimCmd", "-i", "C:\\abc\\input.xml", "-o", "dummy", "-I", "local"};
    args.parse(7, s);
    QCOMPARE(args.inputDir.absolutePath(), QString("C:/abc"));
    QCOMPARE(args.inputFilePath, QString("C:/abc/input.xml"));
}

void TestArgs::testInputDirAbsoluteFileBare() {
    Args args;
    char *s[]  = {"UniSimCmd", "-i", "input.xml", "-o", "dummy", "-I", "C:/abc"};
    args.parse(7, s);
    QCOMPARE(args.inputDir.absolutePath(), QString("C:/abc"));
    QCOMPARE(args.inputFilePath, QString("C:/abc/input.xml"));
}

void TestArgs::testInputDirAbsoluteFileRelative() {
    Args args;
    char *s[]  = {"UniSimCmd", "-i", "here/input.xml", "-o", "dummy", "-I", "C:/abc"};
    args.parse(7, s);
    QCOMPARE(args.inputDir.absolutePath(), QString("C:/abc/here"));
    QCOMPARE(args.inputFilePath, QString("C:/abc/here/input.xml"));
}

void TestArgs::testInputDirAbsoluteFileAbsolute() {
    Args args;
    char *s[]  = {"UniSimCmd", "-i", "C:\\here\\input.xml", "-o", "dummy", "-I", "C:/abc"};
    args.parse(7, s);
    QCOMPARE(args.inputDir.absolutePath(), QString("C:/here"));
    QCOMPARE(args.inputFilePath, QString("C:/here/input.xml"));
}

