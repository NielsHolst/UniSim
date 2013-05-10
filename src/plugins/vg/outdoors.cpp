/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "outdoors.h"

using namespace UniSim;

namespace vg {
	
Outdoors::Outdoors(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameter<double>(Name(co2), 350., "CO2 concentration");
    addVariable<double>(Name(dirRad), "desc");
}

void Outdoors::initialize() {
    Model *records = seekOneChild<Model*>("records");
    globRad = records->pullValuePtr<double>("globRad");
    difRad = records->pullValuePtr<double>("difRad");
}

void Outdoors::reset() {
    update();
}

void Outdoors::update() {
    dirRad = *globRad - *difRad;
}


} //namespace

