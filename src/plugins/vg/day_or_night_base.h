/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_DAY_OR_NIGHT_BASE_H
#define VG_DAY_OR_NIGHT_BASE_H

#include <usbase/model.h>

namespace vg {

class DayOrNightBase : public UniSim::Model
{
public:
    DayOrNightBase(UniSim::Identifier name, QObject *parent);
    void reset();
protected:
    // Variables
    bool isDay;
};
} //namespace


#endif
