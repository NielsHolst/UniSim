/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "greenhouse_transmission.h"

using namespace UniSim;

namespace vg {
	
GreenhouseTransmission::GreenhouseTransmission(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("diffuse", &diffuse, 0.72, this, "Transmission of diffuse light through greenhouse construction [0;1]");
    new Parameter<double>("sinb", &sinb, 1, this, "Sine of sun elevation");
    new Parameter<QString>("swartFileName", &swartFileName, QString("swart.txt"), this, "File with transmission parameters");
    new Variable<double>("direct", &direct, this, "Transmission of direct light through greenhouse construction [0;1]");
}

void GreenhouseTransmission::reset() {
    direct = diffuse;
}

void GreenhouseTransmission::update() {
}


} //namespace

