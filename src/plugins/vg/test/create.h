#include <QString>

namespace UniSim {
    class Simulation;
}

namespace vg { 
	namespace create {
        UniSim::Simulation* simulation(QString fileName);
	}
}
