/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_LINEAR_CONTROL_ELEMENT_H
#define VG_LINEAR_CONTROL_ELEMENT_H

#include "base_control_element.h"

namespace vg {

class LinearControlElement : public BaseControlElement
{
public:
    LinearControlElement(UniSim::Identifier name, QObject *parent);
private:
    // Parameters
    double rate;
    // Methods
    double change(double error);
};
} //namespace


#endif
