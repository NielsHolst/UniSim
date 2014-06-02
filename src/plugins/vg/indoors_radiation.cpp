/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "indoors_radiation.h"
#include "publish.h"

using namespace UniSim;

namespace vg {
	
PUBLISH(IndoorsRadiation)

/*! \class IndoorsRadiation
 * \brief Computes indoors diffuse and direct light
 *
 * Inputs
 * ------
 * - _outdoorsDirectRadiation_ is the direct sunlight irradiation [W/m<SUP>2</SUP>]
 * - _outdoorsDiffuseRadiation_ is the diffuses sunlight irradiation [W/m<SUP>2</SUP>]
 * - _diffuseTransmission_ is the proportion of diffuse sunlight transmitted though the greenhouse cover [0;1]
 * - _directTransmissionAsDirect_ is the proportion of direct sunlight transmitted though the greenhouse cover and remaining direct [0;1]
 * - _directTransmissionAsDiffuse_ is the proportion of direct sunlight transmitted though the greenhouse cover and becoming dispersed as diffuse light [0;1]
 *
 * Outputs
 * ------
 * - _direct_ is the direct light transmitted directly through the greenhouse construction [W/m<SUP>2</SUP>]
 * - _diffuse_ is the total diffuse light transmitted through and dispersed by the greenhouse construction [W/m<SUP>2</SUP>]
 * - _total_ is the total light transmitted through the greenhouse construction [W/m<SUP>2</SUP>]
 *
 * Default dependencies
 * ------------
 * - an _outdoors_ model with two ports:
 *   + _directRadiation_ [W/m<SUP>2</SUP>]
 *   + _diffuseRadiation_ [W/m<SUP>2</SUP>]
 * - an _indoors/screens/transmission_ model with three ports:
 *   + _diffuse_ [0;1]
 *   + _directAsDirect_ [0;1]
 *   + _directAsDiffuse_ [0;1]
 */

IndoorsRadiation::IndoorsRadiation(Identifier name, QObject *parent)
	: Model(name, parent)
{
    InputRef(double, outdoorsDirectRadiation, "outdoors[directRadiation]");
    InputRef(double, outdoorsDiffuseRadiation, "outdoors[diffuseRadiation]");
    InputRef(double, diffuseTransmission, "indoors/screens/transmission[diffuse]");
    InputRef(double, directTransmissionAsDirect, "indoors/screens/transmission[directAsDirect]");
    InputRef(double, directTransmissionAsDiffuse, "indoors/screens/transmission[directAsDiffuse]");
    Output(double, direct);
    Output(double, diffuse);
    Output(double, total);
}

void IndoorsRadiation::reset() {
    direct = diffuse = total = 0.;
}

void IndoorsRadiation::update() {
    diffuse = diffuseTransmission*outdoorsDiffuseRadiation + directTransmissionAsDiffuse*outdoorsDirectRadiation;
    direct = directTransmissionAsDirect*outdoorsDirectRadiation;
    total = direct + diffuse;
}


} //namespace

