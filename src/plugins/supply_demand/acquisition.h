/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef SUPPLYDEMAND_ACQUISITION
#define SUPPLYDEMAND_ACQUISITION
#include <QObject>
#include <usbase/model.h>

namespace SupplyDemand {

class Acquisition : public UniSim::Model
{
	Q_OBJECT
public: 
    Acquisition(UniSim::Identifier name, QObject *parent=0);
	// standard methods
    void initialize();
	void reset();
	void update();

private:
    // methods
    const double* lookupDemand(QString demandName);
    void allocate(double demand, double &available, double &allocation, double &sd);

    // types
    struct Demand {
        const double *pEgestion, *pRespiration, *pReproduction, *pGrowth;
        inline double f(const double *p) { return p ? *p : 0; }
        inline double egestion() { return f(pEgestion); }
        inline double respiration() { return f(pRespiration); }
        inline double reproduction() { return f(pReproduction); }
        inline double growth() { return f(pGrowth); }
    };
    struct Allocation {
        double egestion, respiration, reproduction, growth;
        double sdReproduction, sdGrowth, deficit;
        void setToZero() { egestion = respiration = reproduction = growth = sdReproduction = sdGrowth = deficit = 0.; }
    };

	// parameters
    QString resourceStr;
    double attackRate;

    // pull variables
    Allocation a;
    double demandTotal, supplyTotal;

    // links
    Model *myMassModel, *eggMass, *eggNumber;
    const double *resource, *myMass, *myDuration, *maxGrowthRate;
    double eggWeight;
    Demand d;

};

} //namespace
#endif
