/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include <usbase/utilities.h>
#include "fruit_crop_lai.h"
#include "publish.h"

using std::max;
using namespace UniSim;

namespace vg {

PUBLISH(FruitCropLai)

/*! \class FruitCropLai
 * \brief LAI of fruit crops, such as cucumber and tomato
 *
 * Inputs
 * ------
 * - _maxLai_ is the max. LAI of the crop [m<SUP>2</SUP> leaf/m<SUP>2</SUP> planted area]
 * - _density_ is the number of plants per ground area [m<SUP>-2</SUP>]
 * - _tempSum_ is the temperature sum since the crop was established [d <SUP>o</SUP>C]
 * - _cropPresent_ tells whether the crop is present [true, false]
 *
 *
 * Output
 * ------
 * - _maxLaiReached_ tells whether the max. LAI has been reached [true, false]
 */

FruitCropLai::FruitCropLai(Identifier name, QObject *parent)
    : CropLai(name, parent)
{
    Input(double, maxLai, 3.5);
    InputRef(double, cropDensity, "..[density]");
    InputRef(double, tempSum, "../physTime[total]");
    InputRef(bool, cropPresent, "../periods[flag]");
    Output(bool, maxLaiReached);
}

void FruitCropLai::reset() {
    laiStart = laiStartPerPlant*cropDensity;
    lai = 0.;
    maxLaiReached = (lai>=maxLai);
}

void FruitCropLai::update() {
    lai = cropPresent ? max(laiStart, laiStart+exp(tempSum/700)-1.) : 0.;
    maxLaiReached = (lai>=maxLai);
    if (maxLaiReached)
        lai = maxLai;
}


} //namespace

