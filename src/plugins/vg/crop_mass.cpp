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
 * The densities below all per m<SUP>2</SUP> planted area.
 * - _root_ is root dry mass [g/m<SUP>2</SUP>]
 * - _stem_ is stem dry mass [g/m<SUP>2</SUP>]
 * - _leaf_ is leaf dry mass [g/m<SUP>2</SUP>]
 * - _fruit_ is fruit dry mass [g/m<SUP>2</SUP>]
 * - _total_ is total dry mass [g/m<SUP>2</SUP>]
 *
 * Default dependencies
 * ------------
 * -
 *
 */

CropMass::CropMass(Identifier name, QObject *parent)
	: Model(name, parent)
{
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
}

void CropMass::reset() {
    remove();
}

void CropMass::remove() {
    root = stem = leaf = fruit = total = 0.;
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
    root += propRoot*grossGrowthRate/(1+costRoot);
    stem += propStem*grossGrowthRate/(1+costStem);
    leaf += propLeaf*grossGrowthRate/(1+costLeaf);
    fruit += propFruit*grossGrowthRate/(1+costFruit);
    total = root + stem + leaf + fruit;
}

} //namespace

