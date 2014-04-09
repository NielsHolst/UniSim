/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_CONSTRUCTION_COVER_H
#define VG_CONSTRUCTION_COVER_H

#include <usbase/model.h>
#include <usbase/string_map.h>

namespace vg {

class ConstructionCover : public UniSim::Model
{
public:
    ConstructionCover(UniSim::Identifier name, QObject *parent);
    void amend();
    void update();

    enum Position {Roof1, Roof2, Side1, Side2, End1, End2, Floor};
    Position pullPosition() const;
private:
    // Parameters
    QString type;
    double diffuseTransmission, directTransmissionAtNoon, K, haze;
    bool antiReflection;
    double coverAreaRoof, coverAreaSideWalls, coverAreaEndWalls, groundArea;

    // Variables
    double directTransmission, area;

    // Data
    enum Material {Single, Double, PolyAc};
    struct Cover {
        Material material;
        bool antiReflection;
        double diffuseTransmittance, K, haze;
    };
    typedef UniSim::StringMap<Cover> Covers;
    typedef UniSim::StringMap<Position> Positions;
    static Covers covers;
    static Positions positions;
    Position position;

    // Methods
    void setStandardCovers();
    void setStandardPositions();
};
} //namespace


#endif
