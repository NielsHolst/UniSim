/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_LEAF_LIGHT_RESPONSE_H
#define VG_LEAF_LIGHT_RESPONSE_H


#include <usbase/model.h>

namespace vg {

class LeafLightResponse : public UniSim::Model
{
public:
    LeafLightResponse(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double rsCO2, rbCO2, Tleaf, co2Air,
           rhoChl, theta, frParAbs, concEnzyme;
    // Outputs
    double rtCO2, Pnmax, Pgmax, LUE, Rd;
    // Data
    double x25, TleafK, T25, rhoCo2T,
        VCmax, Jmax, KM, gamma;
    struct MichaelisMenten {
        double KC, KO;
        void update(double x25);
    };
    MichaelisMenten mm;
    // Methods
    double darkRespirationRate();
    double maxCarboxylationRate();
    double maxPhotosyntheticCapacity();
    double potentialLightUseEfficiency();
    double RubiscoCarboxylation();
    double co2CompensationConcentration();
    double maxNetAssimilation();
};
} //namespace


#endif
