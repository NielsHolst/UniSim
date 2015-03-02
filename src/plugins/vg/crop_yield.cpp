/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "crop_yield.h"
#include "publish.h"

using namespace UniSim;

namespace vg {

PUBLISH(CropYield)

/*! \class CropYield
 * \brief CropYield
 *
 * Inputs
 * ------
 * - _produceMass_ is the dry mass of the harvestable produce [g/m<SUP>2</SUP> planted area]
 * - _fractionPlanted_ is the fraction of planted area in the greenhouse [0;1]
 * - _fractionDryWeight_ is the fraction of produce dry weight [0;1]
 *
 * Output
 * ------
 * - _freshWeight_ is the fresh weight of harvestable produce [g/m<SUP>2</SUP> greenhouse area]
 * - _dryWeight_ is the dry weight of harvestable produce [g/m<SUP>2</SUP> greenhouse area]
 *
 * Default dependencies
 * ------------
 */

CropYield::CropYield(Identifier name, QObject *parent)
	: Model(name, parent)
{
    InputRef(double, produceMass, "../mass[fruit]");
    InputRef(double, fractionPlantArea, "..[fractionPlantArea]");
    Input(double, fractionDryWeight, 0.03);
    Output(double, freshWeight);
    Output(double, dryWeight);
}

void CropYield::reset() {
    freshWeight = dryWeight = 0;
}

void CropYield::update() {
    dryWeight = produceMass*fractionPlantArea;
    freshWeight = dryWeight/fractionDryWeight;
}

} //namespace

