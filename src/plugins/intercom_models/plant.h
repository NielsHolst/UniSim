/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INTERCOM_PLANT
#define INTERCOM_PLANT
#include <QList>
#include <QObject>
#include <usbase/model.h>
#include "optimize_allocation.h"

namespace UniSim{
class Stage;
}
namespace intercom{

class Area;
class Mass;
class Organ;

class Plant : public UniSim::Model
{
	Q_OBJECT
public: 
	Plant(UniSim::Identifier name, QObject *parent=0);
	//standard methods
    void initialize();
    void reset();

    // special methods
    void applyEarlyGrowth();
    void accumulate();

    void allocate(double carbohydrates);
    void calcOptimalPartioning();


    double kgPerHa_to_gPerPlant(double x) const;
    double gPerPlant_to_kgPerHa(double x) const;

    static void optimizeSolution();


private:
    // methods
    void updatePullVariables();

    // parameters
    double density;

    // pull variables
    double lai, mass, netAllocation, lightAbsorption, CO2Assimilation,
        grossProduction, maintenanceResp, availableProduction;

    // links
    struct OrganInfo {
        Organ *organ;
        Mass *mass;
        Area *area; //can be null
        const double *massValue, *idealPartitioning, *sla;
        double carbohydrateRequirements;
    };
    QList<OrganInfo> organs;
    QList<UniSim::Stage*> stages;
    UniSim::Model *earlyGrowth;

    // Data
    OptimizeAllocation::Solution solution;
    struct {
        double previous, current;
    } earlyGrowthMass;
};

} //namespace
#endif
