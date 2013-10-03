/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_GREENHOUSE_CONSTRUCTION_H
#define VG_GREENHOUSE_CONSTRUCTION_H

#include <usbase/model.h>
#include <usbase/string_map.h>

namespace vg {

class GreenhouseConstruction : public UniSim::Model
{
public:
    GreenhouseConstruction(UniSim::Identifier name, QObject *parent);
    void initialize();
    typedef enum{Single, Double, Hortiplus} GlassType;
    typedef enum{HPSL, LED} LampType;
private:
    // Parameters
    double width, length, height, roofRatio, sideRatio,
        windowLength, windowHeight, fractionWindows,
        alphaVentilationMax,
        floorHec,
        latcov, Kcover;
    QString strGlassType, strLampType;

    // Variables
    int iGlassType, iLampType;
    double volume, groundArea, coverArea, averageHeight;

    // Methods
    void setGlassType();
    void setLampType();
    void setDimensions();

    // Data
    UniSim::StringMap<GlassType> glassTypes;
    UniSim::StringMap<LampType> lampTypes;
};
} //namespace


#endif
