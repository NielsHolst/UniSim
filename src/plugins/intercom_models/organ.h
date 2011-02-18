/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
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
    void accumulate();
    void setMass(double newMass);
    double allocate(double carbohydrates);
private:
    // methods
    void addMass(double increment);
    void updateMaintenanceRespiration();

	// parameters
    double maintenanceCoeff, CH2ORequirement;

    // pull variables
    double lightAbsorption, CO2Assimilation, maintenanceResp, growthResp;

	// links
    Model *weather, *partition, *area, *mass;
    Plant *plant;
};

} //namespace
#endif
