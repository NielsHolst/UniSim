#include <QDir>
#include <usbase/exception.h>
#include <usengine/simulation.h>
#include <usengine/simulation_maker.h>

using namespace UniSim;

namespace vg {
    namespace create {

        QString filePath(QString fileName) {
            QDir uniSimDir = QDir("../..");
            return uniSimDir.absolutePath() +
                    "/src/plugins/vg/test/input/" +
                    fileName;
        }

        Simulation* simulation(QString fileName) {
            SimulationMaker maker;
            return maker.parse(filePath(fileName));
        }

    }
}
