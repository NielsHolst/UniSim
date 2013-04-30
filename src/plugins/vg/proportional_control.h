/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_PROPORTIONAL_CONTROL_H
#define VG_PROPORTIONAL_CONTROL_H

#include <usbase/model.h>

namespace vg {

class ProportionalControl : public UniSim::Model
{
public:
    ProportionalControl(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Parameters
    double actualValue, targetValue, gapMultiplier, pBand, maxResponse;
    QString targetTypeString;
    enum{Floor, Ceiling} targetType;

    // Variables
    double response;

    // Methods
    void decodeTargetType();
};
} //namespace


#endif
