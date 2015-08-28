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
    double spanWidth, length, height, margin, roofPitch, shade;
    int numSpans;
    bool hasHorizontalScreens;

    // Output
    double width, groundArea,
        roofArea, sideWallsArea, endWallsArea, gablesArea,
        coverArea, coverPerGroundArea,
        indoorsVolume, roofMarginVolume, sideMarginVolume, endMarginVolume,
        wallsMarginVolume, totalVolume,
        roofHeight, roofMarginAverageHeight, indoorsAverageHeight,
        indoorsVolumeProportion, roofMarginVolumeProportion, wallsMarginVolumeProportion;
};
} //namespace


#endif
