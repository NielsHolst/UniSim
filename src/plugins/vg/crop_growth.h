/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_CROP_GROWTH_H
#define VG_CROP_GROWTH_H

#include <usbase/model.h>

namespace vg {

class CropGrowth : public UniSim::Model
{
public:
    CropGrowth(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    double Tcrop, Pg, timeStep,
        massRoot, massStem, massLeaf, massFruit, massTotal,
        respRoot, respStem, respLeaf, respFruit,
        grossGrowthRate, maintenanceRespiration;
    double maintenanceRespirationRate();
};
} //namespace


#endif
