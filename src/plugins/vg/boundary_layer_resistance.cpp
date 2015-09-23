/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "boundary_layer_resistance.h"
#include "publish.h"

using namespace std;
using namespace UniSim;

namespace vg {

PUBLISH(BoundaryLayerResistance)

/*! \class BoundaryLayerResistance
 * \brief Boundary layer resistance against H<SUB>2</SUB>O and CO<SUB>2</SUB>
 *
 * Inputs
 * ------
 * - _ventilation_ is the rate of air exchange through ventilation [h<SUP>-1</SUP>]
 * - _averageHeight_ is the average height of the greenhouse (volume divided by ground area) [m]
 *
 * Outputs
 * ------
 * - _rbH2O_ is the boundary layer resistance against water vapour [s/m]
 * - _rbCO2_ is the boundary layer resistance against CO<SUB>2</SUB> [s/m]
 */

BoundaryLayerResistance::BoundaryLayerResistance(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, leafDimension, 25./1000.);
    InputRef(double, ventilation, "indoors/total/airflux[value]");
    InputRef(double, constructionWidth, "geometry[width]");
    Output(double, rbH2O);
    Output(double, rbCO2);
}

void BoundaryLayerResistance::reset() {
    updateValue(0.);
}

void BoundaryLayerResistance::update() {
    updateValue(ventilation*constructionWidth/3600.); // m/s = h-1 * m *  h/s
    // volume/(length*height) = width
    // See Wang et al. (2000)
}

void BoundaryLayerResistance::updateValue(double windSpeed) {
    // (Stanghellini GCC p 146 modified)
    const double windSpeedMinimum = 0.05;
    rbH2O = 200.*sqrt( leafDimension/max(windSpeedMinimum, windSpeed) );
    rbCO2 = rbH2O*1.37;
}

} //namespace

