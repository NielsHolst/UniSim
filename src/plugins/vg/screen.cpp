/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include "publish.h"
#include "screen.h"

using namespace UniSim;

namespace vg {

PUBLISH(Screen)

//Screen::Positions Screen::positions;
//Screen::Layers Screen::layers;

/*! \class Screen
 * \brief Models different kinds of screens (black-out, energy, shade)
 *
 * Inputs
 * ------
 * - _position_ is one of eight screen positions ["whole_roof", "flat_roof", "roof1", "roof2", "side1", "side2", "end1", "end2"]
 * - _layer_ is one of three layers ["inner", "mid", "outer"]
 * - _lightTransmission_ is the proportion of light transmitted [0;1]
 * - _energyLossReduction_ is the proportion of energy saved by the screen [0;1]
 * - _U50_ is the U-value corresponding to an _energyLossReduction_ of 0.5 [W/m<SUP>2</SUP>/K]
 * - _haze_ is the proportion of direct light becoming dispersed on passage through the material [0;1]
 * - _airTransmission_ is the proportion of air transmitted through the material [0;1]
 * - _state_ is the state of the screen [0;1], where 0=open and 1= drawn
 *
 * Outputs
 * ------
 * - _U_ is calculated from _energyLossReduction_ and _U50_ [W/m<SUP>2</SUP>/K]

 */

Screen::Screen(Identifier name, QObject *parent)
    : Model(name, parent)
{
    Input2(QString, positionStr, position, "");
    Input2(QString, layerStr, layer, "");
    Input(double, lightTransmissivity, 0.41);
    Input(double, lightOuterAbsorptivity, 0.06);
    Input(double, irTransmissivity, 0.);
    Input(double, irInnerEmissivity, 0.62);
    Input(double, irOuterEmissivity, 0.06);


    Input(double, U50, 2.5);
    Input(double, energyLossReduction, 0.4);
    Input(double, haze, 0.);
    Input(double, airTransmission, 0.8);
    Input(double, state, 0.);

    Output(double, U);
}

void Screen::reset() {
    double slope = -U50/50.;
    U = -100*slope + slope*energyLossReduction;
}

} //namespace

