/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_HEATING_FLOW_CONTROLLER_H
#define VG_HEATING_FLOW_CONTROLLER_H

#include <usbase/model.h>

namespace vg {

class HeatingFlowController : public UniSim::Model
{
public:
    HeatingFlowController(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();
private:
    // Parameters
    double heatingSetpoint, indoorsTemperature;
    // Variables
    double signal;
};
} //namespace


#endif
