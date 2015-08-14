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

    enum Position {WholeRoof, FlatRoof, Roof1, Roof2, Side1, Side2, End1, End2};
    enum Layer {Inner, Mid, Outer};
    void reset();
private:
    friend class Screens;

    // Inputs
    QString positionStr, layerStr;
    double lightTransmissivity, lightOuterAbsorptivity,
           irTransmissivity, irOuterEmissivity, irInnerEmissivity,
            energyLossReduction, U50, haze,
            airTransmission, state;
    // Outputs
    double U;
};
} //namespace


#endif
