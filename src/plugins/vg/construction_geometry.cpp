/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QMap>
#include <usbase/exception.h>
#include "construction_geometry.h"

using std::tan;
using std::hypot;
using namespace UniSim;

namespace vg {
	
ConstructionGeometry::ConstructionGeometry(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameter<int>(Name(numSpans), 1, "Number of adjacent spans");
    addParameter<double>(Name(spanWidth), 20., "Width (m)");
    addParameter<double>(Name(length), 50., "Length (m)");
    addParameter<double>(Name(height), 4., "Height (m)");
    addParameter<double>(Name(roofPitch), 26., "Roof pitch (degrees)");

    addVariable<double>(Name(groundArea), "Area (horizontal rectangle) of ground occupied (m2)");
    addVariable<double>(Name(averageHeight), "Average height of greenhouse (m)");

    addVariable<double>(Name(width), "Total width of all spans (m)");
    addVariable<double>(Name(coverAreaSideWalls), " Area (vertical rectangles) of the two sides (m2)");
    addVariable<double>(Name(coverAreaEndWalls), " Area (vertical rectangles) of the two ends (m2)");
    addVariable<double>(Name(coverAreaGables), " Area (vertical triangles) of the gables (m2)");
    addVariable<double>(Name(coverAreaRoof), "Area (sloping rectangles) of the roof sides (m2)");
    addVariable<double>(Name(coverAreaTotal), "Total area of greenhouse cover (m2)");

    addVariable<double>(Name(volumeBelowRoof), " Volume (box) spanned by the four sides (m3)");
    addVariable<double>(Name(volumeRoof), "Volume (prism) of the roof (m3)");
    addVariable<double>(Name(volumeTotal), "Total volume (m3)");

}

void ConstructionGeometry::initialize() {
    double
       roofHeight = tan(roofPitch*PI/180.)*spanWidth/2.,
       roofWidth = hypot(roofHeight, spanWidth/2.);
    width = numSpans*spanWidth;
    groundArea = width*length;
    coverAreaSideWalls = 2*length*height;
    coverAreaEndWalls = 2*width*height;

    coverAreaGables = numSpans*roofHeight*spanWidth,
    coverAreaRoof = 2*numSpans*roofWidth*length;
    coverAreaTotal = coverAreaSideWalls + coverAreaEndWalls + coverAreaGables + coverAreaRoof;

    volumeBelowRoof = groundArea*height;
    volumeRoof = coverAreaGables*length;
    volumeTotal = volumeBelowRoof + volumeRoof;

    averageHeight = volumeTotal/groundArea;
}

} //namespace

