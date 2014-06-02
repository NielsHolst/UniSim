/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_TRESHOLD_VALUE_H
#define VG_TRESHOLD_VALUE_H

#include <usbase/model.h>

namespace vg {

class ThresholdValue : public UniSim::Model
{
public:
    ThresholdValue(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();
protected:
    // Inputs
    double lowValue, highValue, testThreshold, testValue;
    // Outputs
    double value;
};
} //namespace


#endif
