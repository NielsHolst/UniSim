/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "ventilation_by_temp.h"
#include "general.h"
#include "publish.h"

using std::min;
using namespace UniSim;

namespace vg {

PUBLISH(VentilationByTemp)

/*! \class VentilationByTemp
 * \brief Maximum ventilation possible due to the temperature gradient between indoors and outdoors
 * Inputs
 * ------
 * - _indoorsTemperature_ is the indoors tempeature [<SUP>o</SUP>C]
 * - _outdoorsTemperature_ is the outdoors tempeature [<SUP>o</SUP>C]
 * - _windowLength_ is the total length of all vents summed [m]
 * - _windowHeight_ is the average height of all vents [m]
 * - _groundArea_ is the area covered by the greenhouse [m<SUP>2</SUP>]
 *
 * Output
 * ------
 * - _value_ is the maximum ventilation possible due to the temperature gradient between indoors and outdoors [h<SUP>-1</SUP>]
 */

VentilationByTemp::VentilationByTemp(Identifier name, QObject *parent)
    : Model(name, parent)
{
    InputRef(double, indoorsTemperature, "indoors/temperature[value]");
    InputRef(double, outdoorsTemperature, "outdoors[temperature]");
    InputRef(double, windowLength, "construction/vents[totalLength]");
    InputRef(double, windowHeight, "construction/vents[averageHeight]");
    InputRef(double, greenhouseArea, "geometry[groundArea]");
    Input(double, dischargeCoefficient, 0.7);
    Output(double, value);
}

void VentilationByTemp::reset() {
    value = 0.;
}

// De Jong (1990), eq. 6.10, p.83
void VentilationByTemp::update() {
    double dT = indoorsTemperature - outdoorsTemperature;
    double beta = 1./(indoorsTemperature + T0);
    value = dT<0 ? 0. : dischargeCoefficient*windowLength/3*sqrt(g*beta*dT)*pow(windowHeight, 1.5)*3600./greenhouseArea;
}

} //namespace

