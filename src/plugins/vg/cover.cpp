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

Cover::Positions Cover::positions;

/*! \class Cover
 * \brief Characterises the light and energy transmission of a cover part
 *
 * A greenhouse has seven surfaces, each represented by one ConstructionCover object. The name of the object
 * identifies which surface it represents: "Roof1", "Roof2", "Side1", "Side2", "End1", "End2", "Floor".
 *
 * Inputs
 * ------
 * - _U4_ is the heat transfer coefficient of the material at a windspeed of 4 m/s [W/m<SUP>2</SUP>/K]
 * - _emissivity_ is the effectiveness in emitting energy as thermal radiation [0;1].
 * - _absorptivity_ is the proportion of light absorbed [0;1]
 * - _transmissivity_ is the proportion of diffuse light transmitted through the material [0;1]
 * - _density_ is cover density [kg/m<SUP>3</SUP>]
 * - _heatCapacity_ is cover heat capacity [J/kg/K]
 * - _thickness_ is cover thickness [mm]
 * - _haze_ is the proportion of direct light becoming dispersed on passage through the material [0;1]
 * - _antiReflection_ flags whether the material is anti-reflecting [true/false]
 *
 * - _coverAreaRoof_ is the total area of the roof (the two sloping surfaces on top of each span) [m<SUP>2</SUP>]
 * - _coverAreaSideWalls_ is the total area of the two greenhouse side walls (facing the outside) [m<SUP>2</SUP>]
 * - _coverAreaEndWalls_ is the total area of the two greenhouse end walls (excluding the triangular gables) [m<SUP>2</SUP>]
 * - _coverAreaGables_ is the total area of the two triangular gables at the ends of each span [m<SUP>2</SUP>]

 * Output
 * ------
 * - _area_ is the area of this cover [m<SUP>2</SUP>]
 * - _relativeArea_ is the proportional cover of this area out of the total cover [0;1]
 * - _U_ is the wind-corrected heat transfer coefficient of the material [W/m<SUP>2</SUP>/K]
 */

Cover::Cover(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, U4, 7.5);
    Input(double, emissivity, 0.84);
    Input(double, absorptivity, 0.04);
    Input(double, density, 2600.);
    Input(double, heatCapacity, 840.);
    Input(double, thickness, 4.);
    Input(double, haze, 0.);
    Input(bool, antiReflection, 0.);
    Input(double, transmissivity, 1.);

    InputRef(double, coverAreaRoof, "construction/geometry[roofArea]");
    InputRef(double, coverAreaSideWalls, "construction/geometry[sideWallsArea]");
    InputRef(double, coverAreaEndWalls, "construction/geometry[endWallsArea]");
    InputRef(double, coverAreaGables, "construction/geometry[gablesArea]");
    InputRef(double, windspeed, "outdoors[windspeed]");
    Output(double, area);
    Output(double, relativeArea);
    Output(double, U);
    setStandardPositions();
}

void Cover::reset() {
    QString positionName = seekParent<Model*>("*")->id().label();
    position = positions.seek(positionName, this);
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
    }
    relativeArea = area/(coverAreaRoof + coverAreaSideWalls + coverAreaEndWalls + coverAreaGables);
    U = U4;
}

void Cover::update() {
    double k = (windspeed <= 4) ? (2.8 + 1.2*windspeed)/7.6 : pow(windspeed,0.8)/pow(4.,0.8);
    U = k*U4;
}

Cover::Position Cover::pullPosition() const {
    return position;
}

void Cover::setStandardPositions() {
    if (!positions.isEmpty()) return;
    positions["roof1"] = Roof1;
    positions["roof2"] = Roof2;
    positions["side1"] = Side1;
    positions["side2"] = Side2;
    positions["end1"] = End1;
    positions["end2"] = End2;
}

} //namespace

