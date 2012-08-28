/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef ECOTOX_DEPOSITION_MONOTONIC_WEIBULL
#define ECOTOX_DEPOSITION_MONOTONIC_WEIBULL
#include <QObject>
#include <usbase/model.h>

namespace ecotox {

class DepositionMonotonicWeibull : public UniSim::Model
{
	Q_OBJECT
public: 
    DepositionMonotonicWeibull(UniSim::Identifier name, QObject *parent=0);
	// standard methods
    void initialize();
	void reset();
	void update();

private:
    // parameters
    double alpha, beta, c, dateShift;
    QDate dateMax;

    // derived
    double tmax;

    // pull variables
    double value, total;

    // links
    const int *dayOfYear;

    // methods
    double Ptime(int t);
};

} //namespace
#endif
