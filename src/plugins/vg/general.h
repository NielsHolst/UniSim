/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_GENERAL_H
#define VG_GENERAL_H

/*! \file
 *
 */

namespace vg {

const double
    PI = 3.14159,       //!< Mathematical PI
    T0 = 273.15,        //!< Absolute zero offset (oC)
    Sigma = 5.6704e-08, //!< Stefan-Boltzmann constant (W/m2/K4)
    Mwater = 18.016,    //!< Molar mass of water (g/mol)
    Mwa = 0.61,         //!< Molar mass of water relative to molar mass of air (-)
    R = 8.314,          //!< Gas constant (m3 Pa/mol/K)
    P0 = 101325,        //!< Standard atmospheric pressure = 1 atm (Pa)
    CpAir = 1020,       //!< Specific heat capacity of air (J/kg/K)
    CpWater = 4184,     //!< Specific heat capacity of water (J/kg/K)
    RhoAir = 1.19,      //!< Density of air (kg/m3)
    LHe = 2454e3,       //!< Latent heat of vaporisation of water (J/kg)
    Psychr = CpAir*P0/LHe/Mwa;  //!< Psychrometric constant (Pa/K)

double svp(double temperature);
double svpSlope(double temperature);
double saturatedMoistureContent(double temperature);
double vp(double temperature, double rh);
double ah(double temperature, double rh);
double Tdew(double temperature, double rh);
double moistureDeficit(double temperature, double rh);
double ahDeficit(double temperature, double rh);
double rhFromMc(double temperature, double moistureContent);
double rhFromMd(double temperature, double moistureDeficit);
double vpd(double temperature, double rh);
double moistureContent(double temperature, double vp);
double propControl(double input, double pBand, double maxResponse);

} //namespace


#endif
