/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_SLIDING_SIGNAL_H
#define VG_SLIDING_SIGNAL_H

#include "base_signal.h"

namespace vg {

class SlidingSignal : public BaseSignal
{
public:
    SlidingSignal(UniSim::Identifier name, QObject *parent);
    double signal() final;
    virtual double slide(double proportion) = 0;
protected:
    double input, threshold, thresholdBand, signalOutsideBand, signalAtThreshold;
};
} //namespace


#endif
