/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_CROP_H
#define VG_CROP_H

#include <usbase/model.h>
#include <variable_collection.h>

namespace vg {

class VariableCollection;

class Crop : public UniSim::Model
{
public:
    Crop(UniSim::Identifier name, QObject *parent);
    void initialize();
    void reset();
    void update();

private:
    // Parameters
    double kdiff, coverage;

    // Variables
    double absorbedRadiation, temperature, surfaceMoistureContent, transpirationConductance,
        latentHeatFlux, vapourFlux, dewVapourFlux, dew;

    // Data
    VariableCollection ar, t, smc, tc, lhf, vf, dvf, d;
};

} //namespace


#endif
