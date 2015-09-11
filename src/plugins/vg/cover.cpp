/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QMap>
#include <usbase/data_grid.h>
#include <usbase/exception.h>
#include <usbase/interpolate.h>
#include <usengine/simulation.h>
#include "cover.h"
#include "publish.h"

using std::min;
using namespace UniSim;

namespace vg {

PUBLISH(Cover)


/*! \class Cover
 * \brief Characterises the light and energy transmission of a cover part
 *
 * A greenhouse has seven surfaces, each represented by one ConstructionCover object. The name of the object
 * identifies which surface it represents: "Roof1", "Roof2", "Side1", "Side2", "End1", "End2", "Floor".
 *
 * Inputs
 * ------
 * - _greenhouseShade_ is the fraction of light caught by the greenhouse construction [0;1]
 * - _chalk_ is the chalk efficacy [0;1]
 * - _directTransmissionFile_ is the name of a file with coefficients correcting the transmissivity for direct light,
 * according to latitude (rows) and sun azimuth (columns)
 * - _U4_ is the heat transfer coefficient of the material at a windspeed of 4 m/s [W/m<SUP>2</SUP>/K]
 * - _emissivity_ is the effectiveness in emitting energy as thermal radiation [0;1].
 * - _absorptivity_ is the proportion of light absorbed [0;1]
 * - _transmissivity_ is the proportion of diffuse light transmitted through the material [0;1]
 * - _heatCapacity_ is area-specific heat capacity [J/m<SUP>2</SUP>]/K]
 * - _haze_ is the proportion of direct light becoming dispersed on passage through the material [0;1]
 * - _antiReflection_ flags whether the material is anti-reflecting [true/false]
 * - _specificHeatCapacity_ is the area-specific heat capacity [J/m<SUP>2</SUP> cover/K]
 * - _windSpeed_ is the average wind speed [m/s]

 * Output
 * ------
 * - _U_ is the wind-corrected heat transfer coefficient of the material [W/m<SUP>2</SUP>/K]
 * - _heatCapacity_ is the heat capacity [J/K]
 */

Cover::Cover(Identifier name, QObject *parent)
    : SurfaceRadiationOutputs(name, parent)
{
    InputRef(double, greenhouseShade, "geometry[shade]");
    InputRef(double, chalk, "controllers/chalk[signal]");
    Input(QString, directTransmissionFile, "direct_transmission_single.txt");
    InputRef(double, latitude, "calendar[latitude]");
    InputRef(double, azimuth, "calendar[azimuth]");
    InputRef(double, area, "..[area]");
    InputRef(double, windspeed, "outdoors[windspeed]");
    Input(double, U4, 7.5);
    Input(double, emissivity, 0.84);
    Input(double, absorptivity, 0.04);
    Input(double, transmissivity, 1.);
    Input(double, haze, 0.);
    Input(bool, antiReflection, false);
    Input(double, specificHeatCapacity, 840.);

    Output(double, U);
    Output(double, heatCapacity);
}

void Cover::initialize() {
    dirTransTable = new DataGrid(simulation()->inputFilePath(directTransmissionFile), this);
}

void Cover::reset() {
    resetRadiationOutputs();
    U = U4;
}

void Cover::update() {
    double directLightfactor = interpolate(*dirTransTable, latitude, azimuth),
           tr = (1-greenhouseShade)*(1-chalk);
    set( SurfaceRadiation().asCover(tr*transmissivity, tr*transmissivity*directLightfactor, absorptivity, emissivity) );
    double k = (windspeed <= 4) ? (2.8 + 1.2*windspeed)/7.6 : pow(windspeed,0.8)/pow(4.,0.8);
    U = k*U4;
    heatCapacity = specificHeatCapacity*area;
}

} //namespace

