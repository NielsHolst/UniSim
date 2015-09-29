/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_LEAF_PHOTOSYNTHESIS_H
#define VG_LEAF_PHOTOSYNTHESIS_H

#include <Qpair>
#include <usbase/model.h>

namespace vg {

class LeafPhotosynthesis : public UniSim::Model
{
public:
    LeafPhotosynthesis(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double
        kDiffuse, kDirect, kDirectDirect, scattering,
        diffuseReflectivity, directReflectivity,
        parDiffuse, parDirect,
        Pgmax, LUE,
        xGauss, wGauss, lai, Rd, sinB;

    // Outputs
    double absorptivity, parAbsorbed, Pg, Pn;

    // Methods
    double lad() const;
    double laic() const;
    double absorbedByShadedLeaves() const;
    QPair<double,double> absorbedBySunlitLeaves(double absorbedShaded) const;
    double grossAssimilation(double absorbed) const;
};
} //namespace


#endif
