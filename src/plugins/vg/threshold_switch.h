/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_TRESHOLD_SWITCH_H
#define VG_TRESHOLD_SWITCH_H

#include <usbase/model.h>

namespace vg {

class ThresholdSwitch : public UniSim::Model
{
public:
    ThresholdSwitch(UniSim::Identifier name, QObject *parent);
    void reset();
    void initialize();
    void update();
protected:
    // Inputs
    double testThreshold, testValue;
    QString type;
    bool initValue;
    // Outputs
    bool passed;
    // Data
    bool testHigh;
};
} //namespace


#endif
