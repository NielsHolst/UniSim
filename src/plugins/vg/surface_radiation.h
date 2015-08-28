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
        Spectrum() : tra(1) {}
        Spectrum& operator*=(const Spectrum &s2);

        double tra;

        struct Direction {
            Direction() : abs(0), ref(0) {}
            void setRef(double tra);
            void setAbs(double tra);
            double abs, ref;
        } inner, outer;
    } light, ir;

    // Methods
    SurfaceRadiation& asCover(double transmissivity, double absorptivity, double emissivity);
    SurfaceRadiation& asScreen(double transmissivity, double absorptivityIrOuter, double absorptivityIrInner);

    // Operators
    SurfaceRadiation& operator*=(const SurfaceRadiation &s2);
};

} //namespace


#endif
