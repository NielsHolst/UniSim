/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INTERCOM_ORGAN
#define INTERCOM_ORGAN
#include <QObject>
#include <usbase/model.h>

namespace intercom{

class Area;
class Mass;
class Plant;

class Organ : public UniSim::Model
{
	Q_OBJECT
public: 
    Organ(UniSim::Identifier name, QObject *parent=0);
	// standard methods
	void initialize();
    void reset();
	
    // special methods
    //void accumulate();
    void updatePhotosynthesis();
    void allocate(double proportion, double totalCarbohydrates);
    void allocateNet(double proportion, double netCarbohydrates);
private:
    // methods
    void updateMaintenanceRespiration();
    void doAllocate(double totalCarbohydrates, double netCarbohydrates);

	// parameters
    double maintenanceCoeff, CH2ORequirement;

    // pull variables
    double lightAbsorption, CO2Assimilation, grossProduction, maintenanceResp, growthResp, netAllocation,
        allocatedPerPlant, propAllocatedPerPlant;

	// links
    Model *weather, *area, *mass, *community;
    Plant *plant;

    // data
    bool isTestMode;
};

} //namespace
#endif
