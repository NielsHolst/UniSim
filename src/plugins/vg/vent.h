/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_VENT_H
#define VG_VENT_H

#include <usbase/model.h>

namespace vg {

class Vent : public UniSim::Model
{
public:
    Vent(UniSim::Identifier name, QObject *parent);
    void reset();

private:
    // Inputs
    double length, height, number, porosity;
    // Outputs
    double effectiveArea;
};

} //namespace

#endif
