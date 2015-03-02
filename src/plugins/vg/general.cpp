/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <math.h>
#include <usbase/utilities.h>
#include "general.h"

using namespace std;

/*! \file general.cpp
 * General physical functions and constants
 */

namespace vg {

// Amount of water vapour

//! Compute absolute humidity from temperature and specific humidity
/*!	\param temperature (oC)
    \param specific humidity (kg/kg)
    \return absolute humidity (kg/m3)
 */
double ahFromSh(double sh) {
    return sh*RhoAir;
}

//! Compute absolute humidity from temperature and vapour pressure
/*!	\param temperature (oC)
    \param vapour pressure (Pa)
    \return absolute humidity (kg/m3)
 */
double ahFromVp(double temperature, double vp) {
    return vp*Mwater/(R*(temperature + T0));
}

//! Compute specific humidity from temperature and absolute humidity
/*!	\param temperature (oC)
    \param absolute humidity (kg/m3)
    \return specific humidity (kg/kg)
 */
double shFromAh(double ah) {
    return ah/RhoAir;
}

//! Compute specific humidity from temperature and vapour pressure
/*!	\param temperature (oC)
    \param vapour pressure (Pa)
    \return specific humidity (kg/kg)
 */
double shFromVp(double temperature, double vp) {
    double ah = ahFromVp(temperature, vp);
    return shFromAh(ah);
}

//! Compute vapour pressure from temperature and absolute humidity
/*!	\param temperature (oC)
    \param absolute humidity (kg/m3)
    \return vapour pressure (Pa)
 */
double vpFromAh(double temperature, double ah) {
    return ah/Mwater*(R*(temperature + T0));
}

//! Compute vapour pressure from temperature and specific humidity
/*!	\param temperature (oC)
    \param specific humidity (kg/kg)
    \return vapour pressure (Pa)
 */
double vpFromSh(double temperature, double sh) {
    double ah = ahFromSh(sh);
    return vpFromAh(temperature,ah);
}

// Saturated amount of water vapour

//! Compute saturated absolute humidity from temperature
/*!	\param temperature (oC)
    \return absolute humidity (kg/m3)
 */
double sah(double temperature) {
    return ahFromVp(temperature, svp(temperature));
}

//! Compute saturated specific humidity from temperature
/*!	\param temperature (oC)
    \return specific humidity (kg/kg)
 */
double ssh(double temperature) {
    return shFromVp(temperature, svp(temperature));
}

//! Compute saturated water vapour pressure from temperature
/*!	\param temperature (oC)
    \return saturated water vapour pressure (Pa)
 */
double svp(double temperature) {
    double T = temperature + T0;
    return 133.32*exp((1.0887*T - 276.4)/(0.0583*T - 2.1938));
}

//! Compute slope of saturated water vapour pressure curve on temperature
/*!	\param temperature (oC)
    \return saturated water vapour pressure slope, dsvp/dT (Pa/K)
 */
double svpSlope(double temperature) {
    double T = temperature + T0,
        a = 1.088/(0.0583*T - 2.1938),
        b = (1.0887*T - 276.4) / pow(0.0583*T - 2.1938, 2),
        c = (1.0887*T - 276.4) / (0.0583*T - 2.1938);
    return 133.32*(a +  b*0.0583)*exp(c);
}

// Relative measures of water vapour

//! Compute relative humidity from temperature and absolute humidity
/*!	\param temperature (oC)
    \param absolute humidity (kg/m3)
    \return relative humidity [0;100]
 */
double rhFromAh(double temperature, double ah) {
    return 100*ah/sah(temperature);
}

//! Compute relative humidity from temperature and specific humidity
/*!	\param temperature (oC)
    \param specific humidity (kg/kg)
    \return relative humidity [0;100]
 */
double rhFromSh(double temperature, double sh) {
    return 100*sh/ssh(temperature);
}

//! Compute relative humidity from temperature and vapour pressure
/*!	\param temperature (oC)
    \param vapour pressure (Pa)
    \return relative humidity [0;100]
 */
double rhFromVp(double temperature, double vp) {
    return 100*vp/svp(temperature);
}

//! Compute absolute humidity from temperature and relative humidity
/*!	\param temperature (oC)
    \param relative humidity [0;100]
    \return absolute humidity (kg/m3)
 */
double ahFromRh(double temperature, double rh) {
    return rh/100*sah(temperature);
}

//! Compute specific humidity from temperature and relative humidity
/*!	\param temperature (oC)
    \param relative humidity [0;100]
    \return specific humidity (kg/kg)
 */
double shFromRh(double temperature, double rh) {
    return rh/100*ssh(temperature);
}

//! Compute vapour pressure from temperature and relative humidity
/*!	\param temperature (oC)
    \param relative humidity [0;100]
    \return vapour pressure (Pa)
 */
double vpFromRh(double temperature, double rh) {
    return rh/100*svp(temperature);
}

//! Compute absolute humidity Delta X from temperature and relative humidity
/*!	\param temperature (oC)
    \param relative humidity [0;100]
    \return absolute humidity Delta X (kg/m3)
 */
double adxFromRh(double temperature, double rh) {
    double ah = ahFromRh(temperature, rh);
    return sah(temperature) - ah;
}

//! Compute specific humidity Delta X from temperature and relative humidity
/*!	\param temperature (oC)
    \param relative humidity [0;100]
    \return specific humidity Delta X (kg/kg)
 */
double sdxFromRh(double temperature, double rh) {
    double sh = shFromRh(temperature, rh);
    return ssh(temperature) - sh;
}

//! Compute vapour pressure deficit from temperature and relative humidity
/*!	\param temperature (oC)
    \param relative humidity [0;100]
    \return vapour pressure deficit (Pa)
 */
double vpdFromRh(double temperature, double rh) {
    double vp = vpFromRh(temperature, rh);
    return svp(temperature) - vp;
}

double rhFromAdx(double temperature, double adx) {
    double ah = sah(temperature) - adx;
    return rhFromAh(temperature, ah);
}

double rhFromSdx(double temperature, double sdx) {
    double sh = ssh(temperature) - sdx;
    return rhFromSh(temperature, sh);
}

double rhFromVpd(double temperature, double vpd) {
    double vp = svp(temperature) - vpd;
    return rhFromVp(temperature, vp);
}

// CO2

//! Compute ppm CO2 from absolute CO2
/*!	\param temperature (oC)
    \param absolute CO2 (kg/m3)
    \param air pressure (Pa)
    \return ppm CO2 (micromole/mole)
 */
double ppmFromAbsCo2(double temperature, double abs, double P) {
    return abs*R*(temperature+T0)*1e6/P/MCo2;
}

//! Compute absolute CO2 from ppm CO2
/*!	\param temperature (oC)
    \param ppm CO2 (micromole/mole)
    \param air pressure (Pa)
    \return absolute CO2 (kg/m3)
 */
double absFromPpmCo2(double temperature, double ppm, double P) {
    return P*ppm*1e-6/R/(temperature+T0)*MCo2;
}

// Other

//! Compute dew point temperature from temperature and r.h.
/*! Calculated as an inverse function of p_satu.
    \param temperature (oC)
    \param rh relative humidity (%)
    \return dew point temperature (oC)
 */
double Tdew(double temperature, double rh) {
    double vp = vpFromRh(temperature, rh);
    const double k1 = 133.32,
        k2 = 1.0887,
        k3 = 276.4,
        k4 = 0.0583,
        k5 = 2.1938;
    return (k5*log(vp/k1) - k3) / (log(vp/k1)*k4 - k2) - T0;
}

double virtualTemperatureFromAh(double temperature, double ah) {
    return (temperature+T0)*(1+0.608*shFromAh(ah));
}

//! Logistic growth integral
/*!
 * The integral of the logistic growth equation is used to calculate the approach of a variable
 * towards a target value through time
 * \param current is the value at the beginning of the time step
 * \param target is the value approached during the time step
 * \param rate is the rate of approach [inverse units of time step]
 * \param dt is the time step
 * \return the value predicted at the end of the time step
 */
double logistic(double current, double target, double rate, double dt) {
    double L = exp(rate*dt);
    return target*current*L/(target + current*(L-1));
}

double propIntegral(double current, double target, double rate, double dt) {
    double L = exp(rate*dt);
    return target - (target - current)/L;
}

double propExpIntegral(double current, double target, double rate, double dt, double exponent) {
    if (exponent == 1.)
        return(propIntegral(current, target, rate, dt));
    double z = (exponent-1)*rate*dt + pow(fabs(target-current), 1-exponent),
           distance = pow(z, 1/(1-exponent));
    return (current<target) ? target-distance : target+distance;
}

double invPropExpIntegral(double integral, double target, double rate, double dt, double exponent) {
    Q_ASSERT(exponent!=1.);
    double z =  pow(fabs(target-integral), 1-exponent) - (exponent-1)*rate*dt,
           distance = pow(z, 1/(1-exponent));
    return (integral<target) ? target-distance : target+distance;
}

} //namespace

