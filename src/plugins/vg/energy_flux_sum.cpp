/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <qmessagebox.h>
#include "energy_flux_sum.h"
#include "general.h"
#include "publish.h"

using namespace UniSim;

namespace vg {

PUBLISH(EnergyFluxSum)

/*! \class EnergyFluxSum
 * \brief Sum of child model energy fluxes
 *
 * Output
 * ------
 * - _value_ is the sum of energy fluxes [W/m<SUP>2</SUP>]
 *
 * Inputs
 * ------
 * - all EnergyFluxBase child model
 */

EnergyFluxSum::EnergyFluxSum(Identifier name, QObject *parent)
    : EnergyFluxBase(name, parent)
{
}

void EnergyFluxSum::initialize() {
    fluxes.clear();
    auto children = seekChildren<Model*>("*");
    for (Model *model : children)
        fluxes << model->peekValuePtr<double>("value");;
}

void EnergyFluxSum::update() {
    value = 0;
    for (auto flux : fluxes)
        value += *flux;
}

} //namespace

