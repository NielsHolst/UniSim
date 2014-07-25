/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_CROP_BIOMASS_H
#define VG_CROP_BIOMASS_H

#include <usbase/model.h>

namespace vg {

class CropBiomass : public UniSim::Model
{
public:
    CropBiomass(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    double dryWeight, dDryWeight, Tcrop, initDryWeight, Pgc, timeStep, maintenanceRespiration, conversionCost;
    double conversionEfficiency();
    double maintenanceRespirationRate();
};
} //namespace


#endif
