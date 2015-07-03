/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_SCREENS_H
#define VG_SCREENS_H

#include <stdlib.h>
#include <QList>
#include <usbase/model.h>

namespace vg {

class Screens : public UniSim::Model
{
public:
    Screens(UniSim::Identifier name, QObject *parent);
    void initialize();
    void reset();
    void update();
private:
    // Inputs
    double airTransmissionExponent;

    // Outputs
    double maxState, lightTransmission, airTransmission, haze, airTransmissionNot, gap, U;
    // Data
    struct ScreenInfo {
        const double *state, *lightTransmission, *haze, *airTransmission, *U;
        double lightTransmissionTotal() const {
            return (*state)*(*lightTransmission) + 1.-(*state);
        }
        double unhazed() const {
            return 1. - (*state)*(*haze);
        }
        double airTransmissionTotal(double exponent) const {
            double total = (*state)*(*airTransmission) + pow(1.-(*state), exponent);
            return std::min(total, 1.);
        }
        double resistance() const {
            return (*state)/(*U);
        }
    };
    QVector<ScreenInfo> screenInfos;

};
} //namespace


#endif
