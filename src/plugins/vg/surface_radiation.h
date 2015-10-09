/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_SURFACE_RADIATION_H
#define VG_SURFACE_RADIATION_H

namespace vg {

struct SurfaceRadiation {
    SurfaceRadiation();

    struct Spectrum {
        Spectrum(double tra_=1) : tra(tra_) {}
        Spectrum& operator*=(const Spectrum &s2);

        double tra;

        struct Direction {
            Direction() : abs(0), ref(0) {}
            void setRef(double tra);
            void setAbs(double tra);
            double abs, ref;
        } inner, outer;
    } light, directLight, lw;

    // Methods
    void setToZero();
    SurfaceRadiation& asCover(double transmissivity, double directTransmissivity, double absorptivity, double emissivity);
    SurfaceRadiation& asScreen(double transmissivity, double absorptivityLwOuter, double absorptivityLwInner);
    bool isOk();

    // Operators
    SurfaceRadiation& operator*=(const SurfaceRadiation &s2);
};

} //namespace


#endif
