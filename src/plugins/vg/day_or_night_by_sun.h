/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_DAY_OR_NIGHT_BY_SUN_H
#define VG_DAY_OR_NIGHT_BY_SUN_H

#include "day_or_night_base.h"

namespace vg {

class DayOrNightBySun : public DayOrNightBase
{
public:
    DayOrNightBySun(UniSim::Identifier name, QObject *parent);
    void update();
private:
    // Parameters
    double sinb;
};
} //namespace


#endif
