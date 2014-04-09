/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "screen.h"

using namespace UniSim;

namespace vg {

Screen::Positions Screen::positions;
Screen::Layers Screen::layers;

Screen::Screen(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameter<QString>("position", &positionStr, QString(), "Position of screen");
    addParameter<QString>("layer", &layerStr, QString(), "Screen layer");
    addParameter<double>(Name(diffuseTransmission), 0.63, "Transmission of diffuse light through screen [0;1]");
    addParameter<double>(Name(haze), 1., "Haze produced by screen [0;1]; i.e. proportion of direct light becoming dispersed into diffuse light");
    addParameter<double>(Name(K), 5., "K-value");
    setStandardPositions();
    setStandardLayers();
}

void Screen::amend() {
    position = positions.seek(positionStr, this);
    layer = layers.seek(layerStr, this);
}

Screen::Position Screen::pullPosition() const {
    return position;
}

Screen::Layer Screen::pullLayer() const {
    return layer;
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

} //namespace

