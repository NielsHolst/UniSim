/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_FIXED_SIGNAL_H
#define VG_FIXED_SIGNAL_H

#include "base_signal.h"

namespace vg {

class FixedSignal : public BaseSignal
{
public:
    FixedSignal(UniSim::Identifier name, QObject *parent);
    double signal();
private:
    double input;
};

} //namespace
#endif
