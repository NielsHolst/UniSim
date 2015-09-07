/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_LAYER_PHOTOSYNTHESIS_H
#define VG_LAYER_PHOTOSYNTHESIS_H

#include <Qpair>
#include <usbase/model.h>

namespace vg {

class LayerPhotosynthesis : public UniSim::Model
{
public:
    LayerPhotosynthesis(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double
        kDiffuse, kDirect, kDirectDirect, scattering,
        diffuseReflectivity, directReflectivity,
        lightDiffuse, lightDirect, parProportion,
        growthLightPar,
        Pgmax, LUE,
        xGauss, wGauss, lai, Rd, sinB;

    // Test
    double lat;
    int day, hour, minute;

    // Outputs
    double absorptivity, parAbsorbed, Pg, Pn;
    // Data
    double parDiffuse, parDirect;
    // Methods
    double lad() const;
    double laic() const;

    double absorbedByShadedLeaves() const;
    QPair<double,double> absorbedBySunlitLeaves(double absorbedShaded) const;
    double grossAssimilation(double absorbed) const;
};
} //namespace


#endif
