/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QMap>
#include <usbase/exception.h>
#include "construction_ventilation.h"

using std::tan;
using std::hypot;
using namespace UniSim;

namespace vg {
	
ConstructionVentilation::ConstructionVentilation(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameter<double>(Name(ventLength), 200., "Length of vents (m)");
    addParameter<double>(Name(ventMaxOpening), 45., "Max opening angle of vents (degrees)");
    addParameter<double>(Name(ventProportion), 0.1, "Proportion of area that is vents [0;1]");
}


} //namespace

