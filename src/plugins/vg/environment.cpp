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
    try {
    records = seekOneChild<Model*>("records");
    Tair = records->pullValuePtr<double>("Tair");
    windspeed = records->pullValuePtr<double>("windspeed");
    }
    catch (Exception ) {
        co2 = 0;
    }
    throw Exception("test");
}

void Environment::reset() {
}

void Environment::update() {
}


} //namespace

