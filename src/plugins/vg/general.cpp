/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <math.h>
#include "general.h"

using namespace std;

namespace vg {

//! Compute saturated water vapour pressure from temperature
/*!	\param temperature (oC)
    \return saturated water vapour pressure (Pa)
 */
double svp(double temperature) {
    double T = temperature + T0;
    return 133.32*exp((1.0887*T - 276.4)/(0.0583*T - 2.1938));
}

//! Compute water vapour pressure from dry air temperature and humidity
/*!	\param temperature (oC)
    \param rh relative humidity (%)
    \return water vapour pressure (Pa)
 */
double vp(double temperature, double rh) {
    return rh*svp(temperature)/100;
}

//! Compute absolute humidity from dry air temperature and humidity
/*!	\param temperature (oC)
    \param rh relative humidity (%)
    \return absolute humidity (g/g)
 */
double ah(double temperature, double rh) {
    return 18/28.8*(svp(temperature)*rh)/(P0 - svp(temperature)*rh);
}

//! Compute dew point temperature temperature and humidity
/*! Calculated as an inverse function of p_satu.
/*!	\param temperature (oC)
    \param rh relative humidity (%)
    \return dew point temperature (oC)
 */
double Tdew(double temperature, double rh) {
    double vp = vg::vp(temperature, rh);
    const double k1 = 133.32,
        k2 = 1.0887,
        k3 = 276.4,
        k4 = 0.0583,
        k5 = 2.1938;
    return (k5*log(vp/k1) - k3) / (log(vp/k1)*k4 - k2) - T0;
}

//! Compute vapour pressure deficit from temperature and relative humidity
/*!	\param temperature (oC)
    \param rh relative humidity (%)
    \return vapour pressure deficit (Pa)
 */
double vpd(double temperature, double rh) {
    return svp(temperature) - vp(temperature,rh);
}

//! Compute moisture deficit from temperature and humidity
/*!	\param temperature (oC)
    \param rh relative humidity (%)
    \return moisture deficit (g/m3)
 */
double moistureDeficit(double temperature, double rh) {
    return vpd(temperature,rh)*Mwater/(R*(temperature + T0));
}

//! Compute absolute humidity deficit from temperature and humidity
/*!	\param temperature (oC)
    \param rh relative humidity (%)
    \return absolute humidity deficit (g/kg)
 */
double ahDeficit(double temperature, double rh) {
    const double cpAir = 1.2;   // kg/m3 air at standard pressure and temperature
    return moistureDeficit(temperature,rh)/cpAir;
}

//! Compute relative humidity from temperature and moisture deficit
/*!	\param temperature (oC)
    \param moistureDeficit (g/m3)
    \return relative humidity (%)
 */
double rh(double temperature, double moistureDeficit) {
    double T = temperature + T0;
    double xSat = Mwater/(R*T)*svp(T);
    double xGh = xSat - moistureDeficit;
    return 100.*xGh/xSat;
}

//! Compute proportional control
/*! Returns a number in the range [0;maxResponse] increasing with input. If input
    is <= 0 then zero is returned.
    \param input that controls the response (any unit)
    \param pBand that determines swiftness of response (in units of response/input)
    \param maxResponse (in response units)
    \return response to input (in response units)
*/
double propControl(double input, double pBand, double maxResponse) {
    return (input > 0) ? maxResponse*(1 - exp(-input/pBand/maxResponse)) : 0.;
}

} //namespace

