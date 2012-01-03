/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef ECOTOX_DEPOSITION_MONOTONIC_SAMPLED
#define ECOTOX_DEPOSITION_MONOTONIC_SAMPLED
#include <QObject>
#include <usbase/model.h>

namespace ecotox {

class DepositionMonotonicSampled : public UniSim::Model
{
	Q_OBJECT
public: 
    DepositionMonotonicSampled(UniSim::Identifier name, QObject *parent=0);
	// standard methods
    void initialize();
	void reset();
	void update();

private:
    // parameters

    // derived

    // pull variables
    double value, total;

    // links
    const double *airPollen;

    // methods
};

} //namespace
#endif
