/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include <usbase/test_num.h>
#include "fruit_crop_mass.h"
#include "publish.h"

using std::max;
using namespace UniSim;

namespace vg {

PUBLISH(FruitCropMass)

/*! \class FruitCropMass
 * \brief Mass and allocation of fruit crops, such as cucumber, tomato and sweet pepper
 *
 * Inputs
 * ------
 * - _tempSumReproductive_ is the temperature sum when the reproductive stage is reached [d <SUP>o</SUP>C]
 * - _tempMaxReproductive_ is the upper threshold for reproduction; above this value the plant only grows vegetatively [<SUP>o</SUP>C]
 * - _fruitFactor_ is a factor by which _propFruit_ is multiplied [R<SUB>+</SUB>]
 * - _indoorsTemperature_ is the temperature indoors [<SUP>o</SUP>C]
 * - _tempSum_ is the temperature sum determining crop development [d <SUP>o</SUP>C]
 * - _lai_ is the crop LAI [m<SUP>2</SUP>/m<SUP>2</SUP>]
 * - _maxLaiReached_ tells whether the crop has reached its max. LAI [true,false]
 *
 * These inputs apply to the vegetative stage:
 * - _vegPropRoot_ is proportion of growth allocated to roots [0;1]
 * - _vegPropStem_ is proportion of growth allocated to stems [0;1]
 * - _vegPropLeaf_ is proportion of growth allocated to leaves [0;1]
 * - _vegPropFruit_ is proportion of growth allocated to roots [0;1]
 *
 * These inputs apply to the reproductive stage stage:
 * - _vegPropRoot_ is proportion of growth allocated to roots [0;1]
 * - _vegPropStem_ is proportion of growth allocated to stems [0;1]
 * - _vegPropLeaf_ is proportion of growth allocated to leaves [0;1]
 * - _vegPropFruit_ is proportion of growth allocated to roots [0;1]
 *
 * Output
 * ------
 * See CropMass.
 */

FruitCropMass::FruitCropMass(Identifier name, QObject *parent)
    : CropMass(name, parent)
{
    Input(double, vegPropRoot, 0.);
    Input(double, vegPropStem, 0.5);
    Input(double, vegPropLeaf, 0.5);
    Input(double, vegPropFruit, 0.);
    Input(double, reproPropRoot, 0.);
    Input(double, reproPropStem, 0.);
    Input(double, reproPropLeaf, 0.);
    Input(double, reproPropFruit, 1.);
    Input(double, tempSumReproductive, 975.);
    Input(double, tempMaxReproductive, 30.);
    Input(double, fruitFactor, 1.);
    InputRef(double, indoorsTemperature, "indoors/temperature[value]");
    InputRef(double, tempSum, "../physTime[total]");
    InputRef(double, lai, "../lai[value]");
    InputRef(bool, maxLaiReached, "../lai[maxLaiReached]");
}

void FruitCropMass::setProportions() {
    bool isVegetative = (tempSum<tempSumReproductive || indoorsTemperature>tempMaxReproductive);
    if (isVegetative) {
        propRoot = vegPropRoot;
        propLeaf = vegPropLeaf;
        propStem = vegPropStem;
        propFruit = vegPropFruit;
    }
    else {
        propRoot = reproPropRoot;
        propLeaf = reproPropLeaf;
        propStem = reproPropStem;
        propFruit = reproPropFruit;
    }
    // Correct leaf
    if (maxLaiReached)
        propLeaf = 0.;
    // Correct fruit and adjust others
    propFruit *= fruitFactor;
    double nonFruit0 = propRoot + propLeaf + propStem,
           nonFruit = 1. - propFruit;
    if (nonFruit0 > 0) {
        double c = nonFruit/nonFruit0;
        propRoot *= c;
        propStem *= c;
        propLeaf *= c;
    }
    double total = propRoot + propStem + propLeaf + propFruit;
    if (TestNum::ne(total, 1.)) {
        QString msg {"Proportions do not add to 1: root(%1)+stem(%2)+leaf(%3)+fruit(%4)=%5"};
        throw Exception(msg.arg(propRoot).arg(propStem).arg(propLeaf).arg(propFruit).arg(total), this);
    }
}

} //namespace

