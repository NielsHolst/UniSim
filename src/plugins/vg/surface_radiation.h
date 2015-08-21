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
    SurfaceRadiation() : SurfaceRadiation(1,0,0) {}
    SurfaceRadiation(double transmissivity, double absorptivityIrOuter, double absorptivityIrInner);

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

    void updateParameters(double transmissivity, double absorptivityIrOuter, double absorptivityIrInner);
//    SurfaceRadiation& operator=(const SurfaceRadiation &sr);
    SurfaceRadiation& operator*=(const SurfaceRadiation &s2);
};

} //namespace


#endif
