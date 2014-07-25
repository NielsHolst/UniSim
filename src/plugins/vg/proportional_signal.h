/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_PROPORTIONAL_SIGNAL_H
#define VG_PROPORTIONAL_SIGNAL_H

#include "sliding_signal.h"

namespace vg {

class ProportionalSignal : public SlidingSignal
{
public:
    ProportionalSignal(UniSim::Identifier name, QObject *parent);
    double slide(double proportion);
};
} //namespace


#endif
