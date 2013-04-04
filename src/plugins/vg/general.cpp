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
    \param rh as a percentage (0-100)
    \return water vapour pressure (Pa)
 */
double vp(double temperature, double rh) {
    return rh*svp(temperature)/100;
}


//! Compute dew point temperature temperature and humidity
/*! Calculated as an inverse function of p_satu.
    \param temperature in (oC)
    \param rh as a percentage (0-100)
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

//! Compute moisture deficit from temperature and humidity
/*!	\param temperature in (oC)
    \param rh as a percentage (0-100)
    \return moisture deficit (g/m3)
 */
double moistureDeficit(double temperature, double rh) {
    // Actual and saturated vapour pressure (Pa):
    double vp = vg::vp(temperature, rh);
    double svp = vg::svp(temperature);
    // Vapour pressure deficit
    double vpd = svp - vp;
    // Convert to moisture
    return Mwater*vpd/(R*(temperature + T0));
}


} //namespace

