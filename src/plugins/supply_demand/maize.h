/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef SUPPLYDEMAND_MAIZE
#define SUPPLYDEMAND_MAIZE
#include <QObject>
#include <usbase/model.h>

namespace SupplyDemand {

class Maize : public UniSim::Model
{
	Q_OBJECT
public: 
    Maize(UniSim::Identifier name, QObject *parent=0);
	// standard methods
    void initialize();
    void reset();
    void update();

private:
	// parameters
    double alpha, beta, c, minDist, halfDist, halfLife, Pmax, btQuantum, distance, dateShift;
    QDate dateMax;

    // derived
    double tmax, survivalRate;

    // pull variables
    double pollenDepositionRate, pollenDepositionTotal, pollen, toxin;

    // links
    const int *dayOfYear;

    // methods
    double Pdist();
    double Ptime(int t);
};

} //namespace
#endif
