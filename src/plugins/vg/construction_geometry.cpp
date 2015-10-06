/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QMap>
#include <usbase/exception.h>
#include "construction_geometry.h"
#include "publish.h"

using std::tan;
using std::hypot;

using namespace UniSim;

namespace vg {
	
PUBLISH(ConstructionGeometry)
/*! \class ConstructionGeometry
 * \brief Model of the greenhouse construction geometry
 *
 * The greenhouse consists of one or more spans of equal size.
 *
 * Inputs
 * ------
 * - _numSpans_ is the number of spans [1,2,...]
 * - _spanWidth_ is the width of a span [m]
 * - _length_  is the length a span [m]
 * - _height_ is the height of the walls [m]
 * - _roofPitch_ is the degrees slope of the roof [0;180]
 * - _shade_ is the fraction of light caught by the greenhouse construction [0;1]
 * - _screensMaxState_ is the maximum screen state [0;1]
 * - _hasHorizontalScreens_ tells if greenhouse has horizontal screens
 *
 * Outputs
 * -------
 * - _width_ is the width of the greenhouse [m]
 * - _groundArea_ is the area covered by the greenhouse [m<SUP>2</SUP>]
 * - _roofArea_ is the total area of the roof (the two sloping surfaces on top of each span) [m<SUP>2</SUP>]
 * - _sideWallsArea_ is the total area of the two greenhouse side walls (facing the outside) [m<SUP>2</SUP>]
 * - _endWallsArea_ is the total area of the two greenhouse end walls (excluding the triangular gables) [m<SUP>2</SUP>]
 * - _gablesArea_ is the total area of the two triangular gables at the ends of each span [m<SUP>2</SUP>]
 * - _coverArea_ is the area of the green house cover (walls + roof) [m<SUP>2</SUP>]
 * - _coverPerGroundArea_ is _coverArea_ divided by _groundArea_ [m<SUP>2</SUP>/[m<SUP>2</SUP>]]
 * - _indoorsVolume_ is the greenhouse volume, reduced when horizontal screen is drawn [m<SUP>3</SUP>]
 * - _averageHeight_ is the average height of the greenhouse (indoors volume divided by ground area),
 *  reduced when horizontal screen is drawn [m]
 */

ConstructionGeometry::ConstructionGeometry(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(int, numSpans, 1);
    Input(double, spanWidth, 20.);
    Input(double, length, 50.);
    Input(double, height, 4.);
    Input(double, margin, 0.15);
    Input(double, roofPitch, 26.);
    Input(double, shade, 0.1);
    InputRef(double, screensMaxState, "construction/shelters[screensMaxState]");
    InputRef(bool, hasHorizontalScreens, "construction/shelters/roof1/screens[areHorizontal]");

    Output(double, width);
    Output(double, groundArea);
    Output(double, roofArea);
    Output(double, sideWallsArea);
    Output(double, endWallsArea);
    Output(double, gablesArea);
    Output(double, coverArea);
    Output(double, coverPerGroundArea);
    Output(double, indoorsVolume);
    Output(double, indoorsAverageHeight);
}

void ConstructionGeometry::reset() {
    double roofHeight = tan(roofPitch*PI/180.)*spanWidth/2.,
           roofWidth = hypot(roofHeight, spanWidth/2.);
    width = numSpans*spanWidth;
    groundArea = width*length;
    roofArea = 2*numSpans*roofWidth*length;
    sideWallsArea = 2*length*height;
    endWallsArea = 2*width*height;
    gablesArea = numSpans*roofHeight*spanWidth,

    coverArea = sideWallsArea + endWallsArea + gablesArea + roofArea;
    coverPerGroundArea = coverArea/groundArea;

    roofVolume  = length*gablesArea/2.,
    indoorsVolume = groundArea*height + roofVolume;
    indoorsAverageHeight = indoorsVolume/groundArea;
}

void ConstructionGeometry::update() {
    double roofVolumeIndoors =  hasHorizontalScreens ? roofVolume*(1.-screensMaxState) : roofVolume;
    indoorsVolume = groundArea*height + roofVolumeIndoors;
    indoorsAverageHeight = indoorsVolume/groundArea;
}

} //namespace

