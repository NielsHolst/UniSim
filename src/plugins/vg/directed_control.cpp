/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "directed_control.h"

using namespace UniSim;

namespace vg {
	
DirectedControl::DirectedControl(Identifier name, QObject *parent)
	: Model(name, parent)
{
    directions["floor"] = Floor;
    directions["ceiling"] = Ceiling;
    addParameter<QString>("direction", &strDirection, QString("ceiling"),
        "Is target value a lower (floor) or upper (ceiling) limit? "
        "Possible values: " + directions.validKeys().join(", "));
}

void DirectedControl::initialize() {
    _direction = directions.seek(strDirection, this);
}

DirectedControl::Direction DirectedControl::direction() const {
    return _direction;
}

} //namespace

