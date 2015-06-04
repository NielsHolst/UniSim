/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "crop_mass.h"

using namespace UniSim;

namespace vg {

/*! \class CropMass
 * \brief Crop mass and allocation to crop organs
 *
 * Inputs
 * ------
 * - _timeStep_ is the duration of one integration time step [s]
 * - _grossGrowthRate_ is the mass allocated to growth and growth respiration in this time step [g/m<SUP>2</SUP>]
 * - _establishCrop_ tells whether the crop has just been established [true, false]
 * - _removeCrop_ tells whether the crop has just been removed [true, false]
 * - _initMass_ is the initial, total crop dry mass [g/m<SUP>2</SUP>]
 * - _propRoot_ is proportion of growth allocated to roots [0;1]
 * - _propStem_ is proportion of growth allocated to stems [0;1]
 * - _propLeaf_ is proportion of growth allocated to leaves [0;1]
 * - _propFruit_ is proportion of growth allocated to roots [0;1]
 * - _costRoot_ is the relative cost of conversion to root mass (>=0)
 * - _costStem_ is the relative cost of conversion to stem mass (>=0)
 * - _costLeaf_ is the relative cost of conversion to leaf mass (>=0)
 * - _costFruit_ is the relative cost of conversion to fruit mass (>=0)
 *
 * Output
 * ------
 * The densities below are all per m<SUP>2</SUP> planted area.
 * - _root_ is root dry mass [g/m<SUP>2</SUP>]
 * - _stem_ is stem dry mass [g/m<SUP>2</SUP>]
 * - _leaf_ is leaf dry mass [g/m<SUP>2</SUP>]
 * - _fruit_ is fruit dry mass [g/m<SUP>2</SUP>]
 * - _total_ is total dry mass [g/m<SUP>2</SUP>]
 * - _rootGrowthRate_ is the growth rate of root dry mass [g/m<SUP>2</SUP>/h]
 * - _stemGrowthRate_ is the growth rate of stem dry mass [g/m<SUP>2</SUP>/h]
 * - _leafGrowthRate_ is the growth rate of leaf dry mass [g/m<SUP>2</SUP>/h]
 * - _fruitGrowthRate_ is the growth rate of fruit dry mass [g/m<SUP>2</SUP>/h]
 * - _totalGrowthRate_ is the growth rate of total dry mass [g/m<SUP>2</SUP>/h]
 */

CropMass::CropMass(Identifier name, QObject *parent)
	: Model(name, parent)
{
    InputRef(double, timeStep, "calendar[timeStepSecs]");
    InputRef(double, grossGrowthRate, "../growth[grossGrowthRate]");
    InputRef(bool, establishCrop, "../periods[flagUp]");
    InputRef(bool, removeCrop, "../periods[flagDown]");
    Input(double, initMass, 10);
    Input(double, propRoot, 0.);
    Input(double, propStem, 0.5);
    Input(double, propLeaf, 0.5);
    Input(double, propFruit, 0.);
    Input(double, costRoot, 0.39);
    Input(double, costStem, 0.45);
    Input(double, costLeaf, 0.39);
    Input(double, costFruit, 0.35);

    Output(double, root);
    Output(double, stem);
    Output(double, leaf);
    Output(double, fruit);
    Output(double, total);

    Output(double, rootGrowthRate);
    Output(double, stemGrowthRate);
    Output(double, leafGrowthRate);
    Output(double, fruitGrowthRate);
    Output(double, totalGrowthRate);
}

void CropMass::reset() {
    remove();
}

void CropMass::remove() {
    root = stem = leaf = fruit = total =
    rootGrowthRate = stemGrowthRate = leafGrowthRate = fruitGrowthRate = totalGrowthRate = 0.;
}

void CropMass::update() {
    setProportions();
    if (establishCrop)
        establish();
    else if (removeCrop)
        remove();
    allocate();
}

void CropMass::establish() {
    root = propRoot*initMass;
    stem = propStem*initMass;
    leaf = propLeaf*initMass;
    fruit = propFruit*initMass;
    total = root + stem + leaf + fruit;
}

void CropMass::allocate() {
    // Compute growth increments
    rootGrowthRate = propRoot*grossGrowthRate/(1+costRoot);
    stemGrowthRate = propStem*grossGrowthRate/(1+costStem);
    leafGrowthRate = propLeaf*grossGrowthRate/(1+costLeaf);
    fruitGrowthRate = propFruit*grossGrowthRate/(1+costFruit);
    totalGrowthRate = rootGrowthRate + stemGrowthRate + leafGrowthRate + fruitGrowthRate;
    // Add growth increments
    root += rootGrowthRate;
    stem += stemGrowthRate;
    leaf += leafGrowthRate;
    fruit += fruitGrowthRate;
    total += totalGrowthRate;
    // Convert growth increments to rates per hour
    rootGrowthRate *= 3600/timeStep;
    stemGrowthRate *= 3600/timeStep;
    leafGrowthRate *= 3600/timeStep;
    fruitGrowthRate *= 3600/timeStep;
    totalGrowthRate *= 3600/timeStep;
}

} //namespace

