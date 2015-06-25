/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_COVER_H
#define VG_COVER_H

#include <QMap>
#include <usbase/model.h>
#include <usbase/string_map.h>

namespace vg {

class Cover : public UniSim::Model
{
public:
    Cover(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();
    enum Position {Roof1, Roof2, Side1, Side2, End1, End2};
    Position pullPosition() const;
private:
    // Inputs
    QString type;
    double U4, emissivity, absorptivity, transmissivity, density, heatCapacity, thickness,
        haze;
    bool antiReflection;
    double coverAreaRoof, coverAreaSideWalls, coverAreaEndWalls, coverAreaGables,
        windspeed;

    // outputs
    double area, relativeArea, U;

    // Data
    typedef UniSim::StringMap<Position> Positions;
    static Positions positions;
    Position position;

    // Methods
    void setStandardPositions();
};
} //namespace


#endif
