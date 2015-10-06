/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

#include "boundary_layer_resistance_base.h"
#include "publish.h"

using namespace UniSim;

namespace vg {

/*! \class BoundaryLayerResistanceBase
 * \brief Boundary layer resistance against H<SUB>2</SUB>O and CO<SUB>2</SUB>
 *
 * Inputs
 * ------
 * - _leafDimension_ describes leaf geometry [mm]
 * - _indoorsWindSpeed_ is the wind speed indoors [m/s]
 *
 * Outputs
 * ------
 * - _rbH2O_ is the boundary layer resistance against water vapour [s/m]
 * - _rbCO2_ is the boundary layer resistance against CO<SUB>2</SUB> [s/m]
 */

BoundaryLayerResistanceBase::BoundaryLayerResistanceBase(Identifier name, QObject *parent)
    : Model(name, parent)
{
    Input(double, leafDimension, 25./1000.);
    InputRef(double, leafWindSpeed, "../windSpeed[value]");
    Output(double, rbH2O);
    Output(double, rbCO2);
}

void BoundaryLayerResistanceBase::reset() {
    update();
}

void BoundaryLayerResistanceBase::update() {
    setRbH2O();
    rbCO2 = rbH2O*1.37;
}


} //namespace

