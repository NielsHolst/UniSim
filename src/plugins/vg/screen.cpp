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

/*! \class Screen
 * \brief Models different kinds of screens (black-out, energy, shade)
 *
 * Inputs
 * ------
 * - _position_ is one of eight screen positions ["whole_roof", "flat_roof", "roof1", "roof2", "side1", "side2", "end1", "end2"]
 * - _layer_ is one of three layers ["inner", "mid", "outer"]
 * - _transmissivityLight_ is the proportion of light transmitted [0;1]
 * - _emissivityInner_ is the emissivity (=absorptivity) for thermal radiation on the inwards side [0;1]
 * - _emissivityOuter_ is the emissivity (=absorptivity)for thermal radiation on the outwards side [0;1]
 * - _U50_ is the U-value corresponding to an _energyLossReduction_ of 0.5 [W/m<SUP>2</SUP>/K]
 * - _energyLossReduction_ is the proportion of energy saved by the screen [0;1]
 * - _haze_ is the proportion of direct light becoming dispersed on passage through the material [0;1]
 * - _heatCapacity_ is the area-specific heat capacity [J/m<SUP>2</SUP> cover/K]
 * - _transmissivityAir_ is the proportion of air transmitted through the material [0;1]
 * - _transmissivityAirExponent_ corrects for non-linearity of air transmissivity to _state_ [-]
 * - _state_ is the state of the screen [0;1], where 0=open and 1=drawn
 *
 * Outputs
 * ------
 * - _transmissivityLightNet_ is the proportion of light transmitted corrected for _state_ [0;1]
 * - _absorptivityIrInnerNet_ is the absorptivity (=emissivity) for thermal radiation on the inwards side corrected for _state_ [0;1]
 * - _absorptivityIrOuterNet_ is the absorptivity (=emissivity) for thermal radiation on the outwards side corrected for _state_ [0;1]
 * - _unhazed_ is the proportion of direct light passing remaining direct after passage corrected for _state_ [0;1]
 * - _resistance_ is the inverse the U-value corrected for _state_ [K m<SUP>2</SUP>/J]
 * - _transmissivityAirNet_ is the proportion of air transmitted, non-linearly correct for _state_ [0;1]
 * - _isHorizontal_ is the screen in horizontal position ?
 */

Screen::Screen(Identifier name, QObject *parent)
    : Model(name, parent)
{
    Input(QString, position, "");
    Input(QString, layer, "");
    Input(double, transmissivityLight, 0.41);
    Input(double, emissivityInner, 0.62);
    Input(double, emissivityOuter, 0.06);
    Input(double, U50, 2.5);
    Input(double, energyLossReduction, 0.4);
    Input(double, haze, 0.);
    Input(double, heatCapacity, 840.);
    Input(double, transmissivityAir, 0.8);
    Input(double, transmissivityAirExponent, 4.);
    Input(double, state, 0.);

    Output(double, transmissivityLightNet);
    Output(double, absorptivityIrInnerNet);
    Output(double, absorptivityIrOuterNet);
    Output(double, unhazed);
    Output(double, resistance);
    Output(double, transmissivityAirNet);
    Output(bool, isHorizontal);
}

void Screen::reset() {
    isHorizontal = position.toLower().contains("horizontal");
    double slope = -U50/50.;
    U = -100*slope + slope*energyLossReduction;
}

void Screen::update() {
    transmissivityLightNet= 1. - state + state*transmissivityLight;
    absorptivityIrInnerNet = state*emissivityInner;   // Absorptivity = Emissivity for IR
    absorptivityIrOuterNet = state*emissivityOuter;   // do.
    unhazed =  1. - state*haze;
    transmissivityAirNet = std::min( pow(1.-state, transmissivityAirExponent) + state*transmissivityAir, 1. );
    resistance = state/U;
}

} //namespace

