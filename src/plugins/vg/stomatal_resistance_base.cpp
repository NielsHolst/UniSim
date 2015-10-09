/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "general.h"
#include "stomatal_resistance_base.h"

using namespace UniSim;

namespace vg {


/*! \class StomatalResistanceBase
 * \brief Base class for stomatal resistance against H<SUB>2</SUB>O and CO<SUB>2</SUB>
 *
 * Outputs
 * ------
 * - _rbH2O_ is the stomatal resistance against water vapour [s/m]
 * - _rbCO2_ is the stomatal resistance against CO<SUB>2</SUB> [s/m]
 */

StomatalResistanceBase::StomatalResistanceBase(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Output(double, rsH2O);
    Output(double, rsCO2);
}

void StomatalResistanceBase::reset() {
    rsH2O = resetRsH2O();
    rsCO2 = rsH2O*1.6;
}

void StomatalResistanceBase::update() {
    rsH2O = updateRsH2O();
    rsCO2 = rsH2O*1.6;
}

} //namespace

