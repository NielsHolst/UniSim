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
    Mwater = 18.016,    //!< Molar mass of water (g/mol)
    R = 8.316;          //!< Gas constant (J/mol/K)

double svp(double temperature);
double vp(double temperature, double rh);
double Tdew(double temperature, double rh);
double moistureDeficit(double temperature, double rh);

} //namespace


#endif
