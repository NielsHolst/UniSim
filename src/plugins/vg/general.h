/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_GENERAL_H
#define VG_GENERAL_H


namespace vg {

const double
    T0 = 273.15,        //!< Absolute zero offset (oC)
    Sigma = 5.6704e-08, //!< Stefan-Boltzmann constant (W/m2/K4)
    Mwater = 18.016,    //!< Molar mass of water (g/mol)
    R = 8.314,          //!< Gas constant (m3 Pa/mol/K)
    P0 = 101325,        //!< Standard atmosperic pressure = 1 atm (Pa)
    CpAir = 1200,       //!< Specific heat capacity of air (J/m3/K)
    LHe=2454e3;         //!< Latent heat of vaporisation of water (J/kg)

double svp(double temperature);
double vp(double temperature, double rh);
double ah(double temperature, double rh);
double Tdew(double temperature, double rh);
double moistureDeficit(double temperature, double rh);
double ahDeficit(double temperature, double rh);
double rh(double temperature, double moistureDeficit);
double vpd(double temperature, double rh);
double propControl(double input, double pBand, double maxResponse);

} //namespace


#endif
