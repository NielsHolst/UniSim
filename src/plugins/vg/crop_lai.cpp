/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "crop_lai.h"

using namespace UniSim;

namespace vg {

/*! \class CropLai
 * \brief Base class for crop LAI
 *
 * Inputs
 * ------
 * - _laiStartPerPlant_ is the LAI per plant when the crop is established [m<SUP>2</SUP> leaf/m<SUP>2</SUP> planted area]
 *
 * Output
 * ------
 * - _lai_ is the crop LAI [m<SUP>2</SUP> leaf/m<SUP>2</SUP> planted area]
 *
 */

CropLai::CropLai(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, laiStartPerPlant, 0.3);
    Output(double, lai);
}

void CropLai::reset() {
    lai = laiStartPerPlant;
}

} //namespace
