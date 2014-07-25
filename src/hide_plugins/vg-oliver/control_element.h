/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_CONTROL_ELEMENT_H
#define VG_CONTROL_ELEMENT_H

#include "control_element_base.h"

namespace vg {

class ControlElement : public ControlElementBase
{
public:
    ControlElement(UniSim::Identifier name, QObject *parent);
private:
    // Parameters
    double rate;
    // Methods
    double change(double error);
};
} //namespace


#endif
