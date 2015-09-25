/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "general.h"
#include "publish.h"
#include "stomatal_resistance_base.h"

using namespace UniSim;

namespace vg {


/*! \class StomatalResistanceBase
 * \brief Stomatal resistance against H<SUB>2</SUB>O and CO<SUB>2</SUB>
 *
 * Inputs
 * ------
 * - _co2_ is the ambient CO<SUB>2</SUB> concentration [ppm]
 * - _rh_ is indoors relative humidity [0;100]
 * - _Pg_ is gross assimilation rate [g CO<SUB>2</SUB>/m<SUP>2</SUP> leaf/h]
 * - _rbCO2_ is the boundary layer resistance against CO<SUB>2</SUB> [s/m]
 *
 * Outputs
 * ------
 * - _rbH2O_ is the stomatal resistance against water vapour [s/m]
 * - _rbCO2_ is the stomatal resistance against CO<SUB>2</SUB> [s/m]
 *
 * Default dependencies
 * ------------
 * - an _indoors/co2_ model with a _ppm_ port [ppm]
 * - an _indoors/humidity_ model with an _rh_ port [0;100]");
 * - a _photosynthesis_ sibling model with a _Pg_ port [g CO<SUB>2</SUB>/m<SUP>2</SUP> leaf/h]
 * - an _rb_ sibling model with an _rbCO2_ port [s/m]
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

