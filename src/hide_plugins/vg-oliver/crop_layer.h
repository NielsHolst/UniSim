/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_CROP_LAYER_H
#define VG_CROP_LAYER_H

#include <usbase/model.h>

namespace vg {

class CropLayer : public UniSim::Model
{
public:
    CropLayer(UniSim::Identifier name, QObject *parent);
    void initialize();
    void reset();
    void update();
private:
    // Parameters
    double cropLai, kDiff, diffuseRadiation, Tindoors, lowerPipeHeatFlux,
        riH2O, rbH2O, longWaveAbsorption, vpIndoors, vpdIndoors, mcIndoors;

    // Variables
    double absorbedRadiation, temperature, surfaceMoistureContent, transpirationConductance,
        latentHeatFlux, vapourFlux, dewVapourFlux, dew;

    // Data
    int ixLayer, numLayers;
    double lai, laiAbove, s;

    // Methods
    void updateTemperature();
    void updateLatentHeatFlux();
    void updateVapourFlux();
    void updateDewVapourFlux();
    void updateDew();
};
} //namespace


#endif
