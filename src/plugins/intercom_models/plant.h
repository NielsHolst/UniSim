/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INTERCOM_PLANT
#define INTERCOM_PLANT
#include <QList>
#include <QObject>
#include <usbase/model.h>

namespace intercom{

class Area;
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
    double kgPerHa_to_gPerPlant(double x) const;
    double gPerPlant_to_kgPerHa(double x) const;

private:
    // methods
    void updateLai();

    // parameters
    double density;

    // pull variables
    double lai, lightAbsorption, CO2Assimilation,
        grossProduction, maintenanceResp, availableProduction;

    // links
    QList<Area*> areas;
    QList<Organ*> organs;
    QList<Model*> specificLeafAreas, partitions;
    UniSim::Model *earlyGrowth;
};

} //namespace
#endif
