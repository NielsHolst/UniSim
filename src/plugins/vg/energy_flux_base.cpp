/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "energy_flux_base.h"
#include "general.h"
#include "publish.h"

using namespace UniSim;

namespace vg {

PUBLISH(EnergyFluxBase)

/*! \class EnergyFluxBase
 * \brief Base class for energy fluxes to ambient greenhouse air
 *
 * Output
 * ------
 * - _value_ is the energy flux to the greenhouse air per greenhouse area [W/m<SUP>2</SUP>]
 *
 */

EnergyFluxBase::EnergyFluxBase(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Output(double, value);
}

void EnergyFluxBase::reset() {
    value = 0.;
}


} //namespace

