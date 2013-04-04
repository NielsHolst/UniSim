/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_LAMP_HPSL_400_H
#define VG_LAMP_HPSL_400_H

#include "lamp_base.h"

namespace vg {

class LampHpsl400 : public LampBase
{
public:
    LampHpsl400(UniSim::Identifier name, QObject *parent);
    void calculate();
private:
    // Parameters
    bool capacityAsInput;
};
} //namespace


#endif
