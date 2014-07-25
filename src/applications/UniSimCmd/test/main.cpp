#include <iostream>
#include <QtGlobal>
#include <usbase/exception.h>
#include <usbase/test/autotest.h>

using namespace UniSim;

namespace {

    void myMsgHandler(QtMsgType, const QMessageLogContext &context, const QString &msg)
    {
        std::cout << "Message handler called: " << qPrintable(msg)
                  << "From " << context.file << " line " << context.line << " in " << context.function
                  << "\n";
    }

}

int main(int arbc, char *argv[])
{
    qInstallMessageHandler(myMsgHandler);

    QApplication app(arbc, argv);

    QCoreApplication::setOrganizationName("Aarhus University");
    QCoreApplication::setOrganizationDomain("www.agrsci.dk");
    QCoreApplication::setApplicationName("Universal Simulator");

    int result = 0;
    try {
        result = AutoTest::run(0,0);
        QString msg = result==0 ? "All tests passed" : "One or more tests failed";
        std::cout << "\n\n" << qPrintable(msg) << "\n";
    }
    catch (UniSim::Exception &ex) {
        result = -1;
        std::cout << "EXCEPTION: " << qPrintable(ex.message()) << "\n";
    }

    return result;
}

