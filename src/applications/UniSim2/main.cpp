#include <iostream>
#include <QCoreApplication>
#include <usbase/exception.h>
#include <usengine/ast.h>
#include <usengine/file.h>

using namespace UniSim;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Ast ast;
    try {
//        ast.parse(File::filePath("test.uni", File::Input));
        ast.parse("test.uni");
        ast.parseExpression("Simulation sim(a=6){}");
    }
    catch (Exception &ex) {
        std::cout << qPrintable(ex.message());
    }

    return a.exec();
}
