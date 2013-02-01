#include <iostream>
#include <QtGlobal>
#include <usbase/test/autotest.h>
#include <usbase/clock.h>
#include <usbase/exception.h>
#include <usbase/object_pool.h>
#include <usbase/random.h>
#include <usengine/file_locations_strict.h>
#include <usengine/mega_factory.h>
#include "main_window_stub.h"

using namespace UniSim;

namespace {
    void myMsgHandler(QtMsgType, const QMessageLogContext &context, const QString &msg)
    {
        std::cout << "Message handler called: " << qPrintable(msg)
                  << "From " << context.file << " line " << context.line << " in " << context.function
                  << "\n";
    }

    void createSingletons(){
        objectPool()->attach(FileLocations::id(), new FileLocationsStrict);
        objectPool()->attach(MegaFactory::id(), new MegaFactory);
        objectPool()->attach(Random::id(), new Random);
        objectPool()->attach(Clock::id(), new Clock);
        objectPool()->attach("mainWindow", new MainWindowStub);
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
        createSingletons();
        result = AutoTest::run(0,0);
        QString msg = result==0 ? "All tests passed" : "One or more tests failed";
        std::cout << "\n\n" << qPrintable(msg) << "\n";
    }
    catch (UniSim::Exception &ex) {
        std::cout << "EXCEPTION: " << qPrintable(ex.message()) << "\n";
    }

    delete objectPool();

    return result;
}

