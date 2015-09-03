/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "energetics.h"
#include "publish.h"

using namespace UniSim;

namespace vg {

PUBLISH(Energetics)

/*! \class Energetics
 * \brief Compute energy fluxes between components
 *
 * Inputs
 * ------
 * - _leakage_ is infiltration through leakages [m<SUP>3</SUP> air/m<SUP>3</SUP> greenhouse/h]
 * - _averageHeight_ is the average height of the greenhouse [m<SUP>3</SUP>/m<SUP>2</SUP>]
 * - _windspeed_ is the outdoors windspeed [m/s]
 * - _screensAirTransmission_ is the proportion of air led through the screens [0;1]
 *
 * Output
 * ------
 * - _value_ is the proportional air exchange [h<SUP>-1</SUP>]
 */

Energetics::Energetics(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, floorTemperature, 20.);
    Output(double, lightAbsorbedCover);
    Output(double, lightAbsorbedScreens);
}

void Energetics::reset() {
    zeroOutputs();
    lightAbsorbed.clear();
    QList<Model*> shelters = seekMany<Model*>("shelters/*");
    for (Model *shelter : shelters) {
//        Model *cover = shelter->seekOneChild<Model*>("cover"),
//              *screens = shelter->seekOneChild<Model*>("screens");
        lightAbsorbed <<
            Component{
                shelter->pullValuePtr<double>("lightAbsorbedCover"),
                shelter->pullValuePtr<double>("lightAbsorbedScreens")
            };
    }
}

void Energetics::update() {
    zeroOutputs();
    for (Component la : lightAbsorbed) {
        lightAbsorbedCover += (*la.cover);
        lightAbsorbedScreens += (*la.screens);
    }
}

void Energetics::zeroOutputs() {
    lightAbsorbedCover = lightAbsorbedScreens = 0.;
}

} //namespace

