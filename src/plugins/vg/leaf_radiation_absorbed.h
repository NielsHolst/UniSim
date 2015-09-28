/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_LEAF_RADIATION_ABSORBED_H
#define VG_LEAF_RADIATION_ABSORBED_H

#include <usbase/model.h>

namespace vg {

class LeafRadiationAbsorbed : public UniSim::Model
{
public:
    LeafRadiationAbsorbed(UniSim::Identifier name, QObject *parent);
    void initialize();
    void reset();
    void update();
private:
    // Inputs
    double
        xGaussLowerside, wGaussLowerside,
        xGaussUpperside, wGaussUpperside,
        kIr, lai, indoorsLight, heating,
        lightAbsorptivity, emissivity,
        growthLightLight, growthLightLw, growthLightViewFactor,
        leafTemperature, coverTemperature, screensTemperature, screensMaxState,
        shelterOutgoingIrAbsorptivity,
        coverPerGroundArea;

    // Outputs
    double value, lightAbsorbed, heatingAbsorbed,
        growthLightLwAbsorbed,
        shelterLoss;

    // Data
    double irTransmissionLowerside, irTransmissionUpperside;

    struct PipeInfo {
        const double *length, *diameter, *temperature, *emissivity;
        double area() {return (*length)*(*diameter)/1000./2; }  // Only half of the area faces upwards
    };
    QVector<PipeInfo> pipeInfos;

    // Methods
    void setLightAbsorbed();
    void setGrowthLightLwAbsorbed();
    void setShelterLoss();
    void setHeatingAbsorbed();
};

} //namespace

#endif
