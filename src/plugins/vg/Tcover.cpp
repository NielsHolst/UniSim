/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "environment.h"

using namespace UniSim;

namespace vg {
	
Environment::Environment(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("CO2", &co2, 350., this,
    "Outdoors CO2 concentration");

    new Variable<double>("directRadiation", &directRadiation, this, "desc");
    new Variable<double>("Tsky", &Tsky, this, "desc");
    new Variable<double>("Esky", &Esky, this, "desc");
    new Variable<double>("Tcover", &Tcover, this, "desc");
    new Variable<double>("Tscreen", &Tscreen, this, "desc");
    new Variable<double>("Tsoil", &Tsoil, this, "desc");
}

void Environment::initialize() {
    records = seekOneChild<Model*>("records");
    Tair = records->pullValuePtr<double>("Tair");
    windspeed = records->pullValuePtr<double>("windspeed");
}

void Environment::reset() {
}

void Environment::update() {
    double alfa_o = (*windspeed < 4) ?
                1.2*(*windspeed) + 2.8 /* Bot, 1993 */
              : 2.5*pow(*windspeed, 0.8);
    double alfa_i = 3;

    Tcover = ((6+alfa_i)*(*Tair+273.15) + 6*(Tsky+273.15) + alfa_o*(Tout+273.15) + latcov) / (12 + alfa_o + alfa_i);

}


} //namespace

