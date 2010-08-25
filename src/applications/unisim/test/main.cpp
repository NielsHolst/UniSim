#include <iostream>
#include <usbase/test/autotest.h>
#include <usbase/clock.h>
#include <usbase/exception.h>
#include <usbase/object_pool.h>
#include <usengine/integrator_maker.h>
#include <usengine/file_locations_strict.h>
#include <usengine/model_maker.h>
#include <usengine/output_maker.h>

using namespace UniSim;

void myMsgHandler(QtMsgType type, const char *msg)
{
    std::cout << "Message handler called: " << msg << "\n";
}

void createSingletons(){
    objectPool()->attach(FileLocations::id(), new FileLocationsStrict);
    objectPool()->attach(IntegratorMaker::id(), new IntegratorMaker);
    objectPool()->attach(ModelMaker::id(), new ModelMaker);
    objectPool()->attach(OutputMaker::id(), new OutputMaker);
    //objectPool()->attach(RandomUniform::id(), new RandomUniform);
    objectPool()->attach(Clock::id(), new Clock);
}

int main(int arbc, char *argv[])
{
    qInstallMsgHandler(myMsgHandler);

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

