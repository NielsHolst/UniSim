/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include "control_element.h"
#include "publish.h"
#include "screen.h"

using namespace UniSim;

namespace vg {

PUBLISH(Screen)

Screen::Positions Screen::positions;
Screen::Layers Screen::layers;

/*! \class Screen
 * \brief Models different kinds of screens (black-out, energy, shade)
 *
 * Inputs
 * ------
 * - _position_ is one of eight screen positions
 * ["whole_roof", "flat_roof", "roof1", "roof2", "side1", "side2", "end1", "end2"]
 * - _layer_ is one of three layers ["inner", "mid", "outer"]
 * - _lightTransmission_ is the proportion of light transmitted [0;1]
 * - _energyLossReduction_ is the proportion of energy saved by the screen [0;1]
 * - _haze_ is the proportion of direct light becoming dispersed on passage through the material [0;1]
 * - _airTransmission_ is the proportion of air transmitted through the material [0;1]
 *
 * Outputs
 * ------
 * - none

 * Obligatory dependencies
 * ------------
 * - one child model of type ControlElement
 */

Screen::Screen(Identifier name, QObject *parent)
    : Model(name, parent), _state(0)
{
    Input2(QString, positionStr, position, "");
    Input2(QString, layerStr, layer, "");
    Input(double, lightTransmission, 0.63);
    Input(double, energyLossReduction, 0.4);
    Input(double, haze, 1.);
    Input(double, airTransmission, 0.8);
    setStandardPositions();
    setStandardLayers();
}

Screen::Position Screen::pullPosition() const {
    return positions.seek(positionStr, this);;
}

Screen::Layer Screen::pullLayer() const {
    return layers.seek(layerStr, this);
}

void Screen::setStandardPositions() {
    if (!positions.isEmpty()) return;
    positions["whole_roof"] = WholeRoof;
    positions["flat_roof"] = FlatRoof;
    positions["roof1"] = Roof1;
    positions["roof2"] = Roof2;
    positions["side1"] = Side1;
    positions["side2"] = Side2;
    positions["end1"] = End1;
    positions["end2"] = End2;

}

void Screen::setStandardLayers() {
    if (!layers.isEmpty()) return;
    layers["inner"] = Inner;
    layers["mid"] = Mid;
    layers["outer"] = Outer;
}

const double *Screen::state() {
    // Lazy pull of state
    if (_state) return _state;
    auto control = peekOneChild<ControlElement*>("*");
    if (!control)
        throw Exception("Screen model must have one child of type 'ControlElement'");
    return _state = control->pullValuePtr<double>("state");
}

} //namespace

