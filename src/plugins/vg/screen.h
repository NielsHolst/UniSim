/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_SCREEN_H
#define VG_SCREEN_H

#include <usbase/model.h>
#include <usbase/string_map.h>

namespace vg {

class Screen : public UniSim::Model
{
public:
    Screen(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();
private:
    friend class Screens;

    // Inputs
    QString position, layer;
    double shelterArea,
           transmissivityLight,
           emissivityInner, emissivityOuter,
           specificHeatCapacity,
           energyLossReduction, U, haze,
           transmissivityAir, transmissivityAirExponent, state;
    // Outputs
    double
        transmissivityLightNet,
        absorptivityIrInnerNet, absorptivityIrOuterNet,
        unhazed, transmissivityAirNet, effectiveArea,
        resistance, heatCapacity;
    bool isHorizontal;
    // Methods
    void updateByState(double state);
};

} //namespace


#endif
