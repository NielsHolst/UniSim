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
    void reset();
    typedef enum{Single, Double, Hortiplus} GlassType;
    typedef enum{HPSL, LED} LampType;
private:
    // Input
    double spanWidth, length, height, roofPitch, shade;
    int numSpans;

    // Output
    double width, groundArea, averageHeight,
        coverArea, coverPerGroundArea, sideWallsArea, endWallsArea,
        roofArea, roofHeight, roofAverageHeight, gablesArea,
        volume, volumeIndoors, volumeTop, volumeProportionIndoors, volumeProportionTop;
};
} //namespace


#endif
