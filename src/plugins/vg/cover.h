/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_COVER_H
#define VG_COVER_H

#include <usbase/model.h>
#include <usbase/string_map.h>

namespace vg {

class Cover : public UniSim::Model
{
public:
    Cover(UniSim::Identifier name, QObject *parent);
    void initialize();

    enum Position {Roof1, Roof2, Side1, Side2, End1, End2, Floor};
    Position pullPosition() const;
private:
    // Parameters
    QString type;
    double diffuseTransmission, U, haze;
    bool antiReflection;
    double coverAreaRoof, coverAreaSideWalls, coverAreaEndWalls, coverAreaGables, groundArea;

    // Variables
    double area;

    // Data
    enum Material {Single, Double, PolyAc};
    struct Type {
        Material material;
        bool antiReflection;
        double diffuseTransmittance, U, haze;
    };
    typedef UniSim::StringMap<Type> Types;
    typedef UniSim::StringMap<Position> Positions;
    static Types types;
    static Positions positions;
    Position position;

    // Methods
    void setStandardTypes();
    void setStandardPositions();
};
} //namespace


#endif
