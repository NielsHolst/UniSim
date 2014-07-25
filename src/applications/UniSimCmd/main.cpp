#include <iostream>
#include <QApplication>
#include <QCoreApplication>
#include <usbase/authors.h>
#include <usbase/clock.h>
#include <usbase/component.h>
#include <usbase/exception.h>
#include <usbase/object_pool.h>
#include <usbase/random.h>
#include <usengine/mega_factory.h>
#include <usengine/simulation.h>
#include <usengine/simulation_maker.h>
#include "args.h"
#include "file_locations_args.h"
#include "main_window_stub.h"


using namespace std;
using namespace UniSim;

void myMsgHandler(QtMsgType, const QMessageLogContext &, const QString &msg) {
    cout << qPrintable(msg);
}

void createSingletons(const Args &args){
    objectPool()->attach(Clock::id(), new Clock);
    objectPool()->attach(FileLocations::id(), new FileLocationsArgs(args));
    objectPool()->attach(MegaFactory::id(), new MegaFactory);
    objectPool()->attach(Random::id(), new Random);
    objectPool()->attach("mainWindow", new MainWindowStub);
}

void runSimulation(QString inputFilePath, QString outputFileName) {
    SimulationMaker *maker = new SimulationMaker();
    Simulation *simulation;
    try {
        simulation = maker->parse(inputFilePath);
        Component *output = simulation->seekOneChild<Component*>("output");
//        output->pushValue<QString>("fileName", outputFileName);
        output->seekOneChild<Parameter<QString>*>("fileName") -> setValue(outputFileName);
        simulation->execute();
    }
    catch (UniSim::Exception &ex) {
        throw;
    }

    delete maker;
    delete simulation;
}

int main(int argc, char *argv[]) {
    qInstallMessageHandler(myMsgHandler);
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("Aarhus University");
    QCoreApplication::setOrganizationDomain("www.agrsci.dk");
    QCoreApplication::setApplicationName("Universal Simulator");

    Args args;
    args.parse(argc, argv);
    cout    << "\n\nReading from "
            << qPrintable(args.inputFilePath) << "\n"
            << "Using plugin DLLs in "
            << qPrintable(args.pluginsDir.absolutePath()) << "\n"
            << "Writing to "
            << qPrintable(args.outputFilePath) << "\n"
            << "Placing temporary files in "
            << qPrintable(args.tempDir.absolutePath()) << "\n\n";
    int result{0};
    try {
        createSingletons(args);
        runSimulation(args.inputFilePath, args.outputFileName);
    }
    catch (Exception &ex) {
        cout << qPrintable("Program Error: " + ex.message());
        result = 1;
    }
    catch (...) {
        cout << "Program Error. Uncaught exception.";
        result = 1;
    }
    delete objectPool();
    return result;
}
