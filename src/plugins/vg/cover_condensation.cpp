
/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "cover_condensation.h"
#include "general.h"
#include "publish.h"

using namespace UniSim;
using std::max;

namespace vg {

PUBLISH(CoverCondensation)
/*! \class CoverCondensation
 * \brief Condensation on greenhouse cover
 *
 * Inputs
 * ------
 * - _coverArea_ is the area of the greenhouse cover [m<SUP>2</SUP>]
 * - _groundArea_ is the ground area occupied by the greenhouse cover [m<SUP>2</SUP>]
 * - _indoorsTemperature_ is the ambient temperature indoors [<SUP>o</SUP>C]
 * - _indoorsAh_ is the indoors absolute humidity [kg/m<SUP>3</SUP>]
 * - _coverTemperature_ is the temperature of the greenhouse cover [<SUP>o</SUP>C]
 *
 * Outputs
 * ------
 * - see VapourFluxBase
 */

CoverCondensation::CoverCondensation(Identifier name, QObject *parent)
    : VapourFluxBase(name, parent)
{
    InputRef(double, coverArea, "..[area]");
    InputRef(double, coverTemperature, "../energyFlux[temperature]");
    InputRef(double, groundArea, "construction/geometry[groundArea]");
    InputRef(double, indoorsTemperature, "indoors/temperature[value]");
    InputRef(double, indoorsAh, "indoors/humidity[ah]");
}

void CoverCondensation::update() {
    double dTemp = indoorsTemperature - coverTemperature;
    conductance = dTemp > 0
                  ? coverArea/groundArea*1.64e-3*pow(dTemp, 1/3.) // GCC, p.147
                  : 0.;
	double coverSah = sah(coverTemperature);
    vapourFlux = max(conductance*(indoorsAh - coverSah), 0.);
    gain = conductance*coverSah;
}

} //namespace

