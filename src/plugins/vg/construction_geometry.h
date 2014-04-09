/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_CONSTRUCTION_GEOMETRY_H
#define VG_CONSTRUCTION_GEOMETRY_H

#include <usbase/model.h>
#include <usbase/string_map.h>

namespace vg {

class ConstructionGeometry : public UniSim::Model
{
public:
    ConstructionGeometry(UniSim::Identifier name, QObject *parent);
    void initialize();
    typedef enum{Single, Double, Hortiplus} GlassType;
    typedef enum{HPSL, LED} LampType;
private:
    // Parameters
    double spanWidth, length, height, roofPitch;
    int numSpans;

    // Variables
    double width, groundArea, averageHeight,
        coverAreaTotal, coverAreaSideWalls, coverAreaEndWalls, coverAreaRoof, coverAreaGables,
        volumeTotal, volumeBelowRoof, volumeRoof;
};
} //namespace


#endif
