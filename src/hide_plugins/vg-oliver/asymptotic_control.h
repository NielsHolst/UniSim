/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_ASYMPTOTIC_CONTROL_H
#define VG_ASYMPTOTIC_CONTROL_H

#include "directed_control.h"
#include <usbase/identifier.h>

namespace vg {

class AsymptoticControl : public DirectedControl
{
public:
    AsymptoticControl(UniSim::Identifier name, QObject *parent);
    void update();
};

} //namespace


#endif
