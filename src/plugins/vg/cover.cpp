/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QMap>
#include <usbase/exception.h>
#include "cover.h"
#include "publish.h"

using std::min;
using namespace UniSim;

namespace vg {

PUBLISH(Cover)

Cover::Types Cover::types;
Cover::Positions Cover::positions;

/*! \class Cover
 * \brief Characterises the light and energy transmission of a cover part
 *
 * A greenhouse has seven surfaces, each represented by one ConstructionCover object. The name of the object
 * identifies which surface it represents: "Roof1", "Roof2", "Side1", "Side2", "End1", "End2", "Floor".
 *
 * Inputs
 * ------
 * - _type_ denotes the cover material given as a string:
 *   "user-defined""enkelt", "single", "double", "polyac", etc.
 *   Unless _type_ is "user-defined" then the inputs _diffuseTransmission_, _U_, _haze_
 *   and _antiReflection_ will be over-written with values according to the set _type_
 * - _diffuseTransmission_ is the proportion of diffuse light transmitted through the material [0;1]
 * - _U_ is the heat transfer coefficient of the material [W/m<SUP>2</SUP>/K]
 * - _haze_ is the proportion of direct light becoming dispersed on passage through the material [0;1]
 * - _antiReflection_ flags whether the material is anti-reflecting [true/false]

 * Output
 * ------
 * - _area_ is the area of this cover [m<SUP>2</SUP>]
 *
 * Default dependencies
 * ------------
 * - a _construction/geometry_ model with these ports
 *   + roofArea [m<SUP>2</SUP>]
 *   + sideWallsArea [m<SUP>2</SUP>]
 *   + endWallsArea [m<SUP>2</SUP>]
 *   + gablesArea [m<SUP>2</SUP>]
 *   + groundArea [m<SUP>2</SUP>]
 */

Cover::Cover(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(QString, type, "");
    Input(double, U, 0.);
    Input(double, haze, 0.);
    Input(bool, antiReflection, 0.);
    Input(double, diffuseTransmission, 1.);
    InputRef(double, coverAreaRoof, "construction/geometry[roofArea]");
    InputRef(double, coverAreaSideWalls, "construction/geometry[sideWallsArea]");
    InputRef(double, coverAreaEndWalls, "construction/geometry[endWallsArea]");
    InputRef(double, coverAreaGables, "construction/geometry[gablesArea]");
    InputRef(double, groundArea, "construction/geometry[groundArea]");
    Output(double, area);
    setStandardTypes();
    setStandardPositions();
}

void Cover::initialize() {
    position = positions.seek(id().label(), this);
    if (type.size()>0 && type.toLower()!="user-defined") {
        Type myType = types.seek(type, this);
        diffuseTransmission = myType.diffuseTransmittance;
        U = myType.U;
        haze = myType.haze;
        antiReflection = myType.antiReflection;
    }
    switch (position) {
    case Roof1:
    case Roof2:
        area = coverAreaRoof/2;
        break;
    case Side1:
    case Side2:
        area = coverAreaSideWalls/2;
        break;
    case End1:
    case End2:
        area = coverAreaEndWalls/2 + coverAreaGables/2;
        break;
    case Floor:
        area = groundArea;
        break;
    }
}

Cover::Position Cover::pullPosition() const {
    return position;
}

void Cover::setStandardTypes() {
    if (!types.isEmpty()) return;
    const double T = 1.098,
        D = 0.831/0.915, //Calculated from SUNARC measurements: double AR_20/single AR_20
        DH = 37./16;     //Calculated from SUNARC measurements: double AR_20/single AR_20
    types["enkelt"] =
    types["single"] = Type{Single, false, 0.8/T, 7.5, 0};
    types["double"] = Type{Double, false, 0.8/T*D, 6, 0};
    types["polyac"] = Type{PolyAc, false, 0.7/T, 4.5, 0};
    types["s20snr"] = Type{Single, false, 0.844/T, 7.5, 0};
    types["s20sar"] = Type{Single, true, 0.924/T, 7.5, 0.16};
    types["s50snr"] = Type{Single, false, 0.832/T, 7.5, 0.46};
    types["s50sar"] = Type{Single, true, 0.910/T, 7.5, 0.47};
    types["s70snr"] = Type{Single, false, 0.80/T, 7.5, 0.77};
    types["s70sar"] = Type{Single, true, 0.874/T, 7.5, 0.75};
    types["s20dnr"] = Type{Double, false, 0.843/T*D, 6.0, 0.37};
    types["s20dar"] = Type{Double, true, 0.831/T*D, 6.0, 0.37};
    types["s50dnr"] = Type{Double, false, 0.832/T*D, 6.0, 0.46*1.5};
    types["s50dar"] = Type{Double, true, 0.91/T*D, 6.0, min(0.75,0.47*DH)};
    types["s70dnr"] = Type{Double, false, 0.8/T*D, 6.0, min(0.95,0.77*DH)};
    types["s70dar"] = Type{Double, true, 0.874/T*D, 6.0, min(0.95,0.75*DH)};
}

void Cover::setStandardPositions() {
    if (!positions.isEmpty()) return;
    positions["roof1"] = Roof1;
    positions["roof2"] = Roof2;
    positions["side1"] = Side1;
    positions["side2"] = Side2;
    positions["end1"] = End1;
    positions["end2"] = End2;
    positions["floor"] = Floor;
}

} //namespace

