/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QMap>
#include <usbase/exception.h>
#include "construction_cover.h"

using std::min;
using namespace UniSim;

namespace vg {

ConstructionCover::Covers ConstructionCover::covers;
ConstructionCover::Positions ConstructionCover::positions;

ConstructionCover::ConstructionCover(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameter<QString>(Name(type), QString("Single"), "Name of cover type ("+ covers.validKeys().join(",") +")or @F User-defined");
    addParameter<double>(Name(diffuseTransmission), 1., "Proportion of diffuse light transmitted through cover [0;1]");
    addParameter<double>(Name(directTransmissionAtNoon), 0.85,
                         "Max. proportion of direct light transmitted through screen [0;1], i.e. at noon");
    addParameter<double>(Name(K), 0., "K-value of cover");
    addParameter<double>(Name(haze), 0., "Haze produced by cover [0;1]; i.e. proportion of direct light becoming dispersed into diffuse light");
    addParameter<bool>(Name(antiReflection), false, "Is cover anti-reflective?");

    addParameterRef<double>(Name(coverAreaRoof), "construction/geometry[coverAreaRoof]");
    addParameterRef<double>(Name(coverAreaSideWalls), "construction/geometry[coverAreaSideWalls]");
    addParameterRef<double>(Name(coverAreaEndWalls), "construction/geometry[coverAreaEndWalls]");
    addParameterRef<double>(Name(groundArea), "construction/geometry[groundArea]");

    addVariable<double>(Name(directTransmission), "Proportion of direct light transmitted through cover [0;1]");
    addVariable<double>(Name(area), "Cover area (m2)");

    setStandardCovers();
    setStandardPositions();
}

void ConstructionCover::amend() {
    position = positions.seek(id().label(), this);
    if (type.toLower()!="user-defined") {
        Cover c = covers.seek(type, this);
        diffuseTransmission = c.diffuseTransmittance;
        K = c.K;
        haze = c.haze;
        antiReflection = c.antiReflection;
    }
    switch (position) {
    case Roof1:
    case Roof2:
        area = coverAreaRoof;
        break;
    case Side1:
    case Side2:
        area = coverAreaSideWalls/2;
        break;
    case End1:
    case End2:
        area = coverAreaEndWalls/2;
        break;
    case Floor:
        area = groundArea;
        break;
    }
}

void ConstructionCover::update() {
    // update directTransmittance according to azimuth
}

ConstructionCover::Position ConstructionCover::pullPosition() const {
    return position;
}

void ConstructionCover::setStandardCovers() {
    if (!covers.isEmpty()) return;
    const double T = 1.098,
        D = 0.831/0.915, //Calculated from SUNARC measurements: double AR_20/single AR_20
        DH = 37./16;     //Calculated from SUNARC measurements: double AR_20/single AR_20
    covers["enkelt"] =
    covers["single"] = Cover{Single, false, 0.8/T, 7.5, 0};
    covers["double"] = Cover{Double, false, 0.8/T*D, 6, 0};
    covers["polyac"] = Cover{PolyAc, false, 0.7/T, 4.5, 0};
    covers["s20snr"] = Cover{Single, false, 0.844/T, 7.5, 0};
    covers["s20sar"] = Cover{Single, true, 0.924/T, 7.5, 16};
    covers["s50snr"] = Cover{Single, false, 0.832/T, 7.5, 46};
    covers["s50sar"] = Cover{Single, true, 0.910/T, 7.5, 47};
    covers["s70snr"] = Cover{Single, false, 0.80/T, 7.5, 77};
    covers["s70sar"] = Cover{Single, true, 0.874/T, 7.5, 75};
    covers["s20dnr"] = Cover{Double, false, 0.843/T*D, 6.0, 37};
    covers["s20dar"] = Cover{Double, true, 0.831/T*D, 6.0, 37};
    covers["s50dnr"] = Cover{Double, false, 0.832/T*D, 6.0, 46*1.5};
    covers["s50dar"] = Cover{Double, true, 0.91/T*D, 6.0, min(75.,47*DH)};
    covers["s70dnr"] = Cover{Double, false, 0.8/T*D, 6.0, min(95.,77*DH)};
    covers["s70dar"] = Cover{Double, true, 0.874/T*D, 6.0, min(95.,75*DH)};
}

void ConstructionCover::setStandardPositions() {
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

