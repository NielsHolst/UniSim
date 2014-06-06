/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <QMapIterator>
#include <usbase/data_grid.h>
#include <usbase/interpolate.h>
#include <usengine/simulation.h>
#include "cover.h"
#include "screen.h"
#include "transmission_base.h"

using namespace UniSim;

namespace vg {
	
/*! \class TransmissionBase
 * \brief Greenhouse light and energy transmission through cover and screens
 *
 * Inputs
 * ------
 * - _directTransmissionFile_ is a table with transmission values [0;1]
 * according to latitude (rows) and sun azimuth (columns)
 * - _latitude_ is the geographical latitude of the greenhouse [-180;180]
 * - _azimuth_ is the sun azimuth [-90;90]
 * - _greenhouseSurfaceArea_ is the area of the greenhouse surface (walls + roof) [m<SUP>2</SUP>]
 * - _greenhouseGroundArea_ is the grounf area covered by the greenhouse [m<SUP>2</SUP>]
 *
 * Outputs
 * -------
 * - _U_ is the common U per greenhouse surface area [W/K/m<SUP>2</SUP>]
 * - _netU_ is the common U per greenhouse ground area [W/K/m<SUP>2</SUP>]
 * - _haze_ is the proportion of direct light becoming dispersed on passage through the whole greenhouse surface [0;1]
 * - _diffuse_ is the proportion of diffuse light transmitted through the whole greenhouse surface [0;1]
 * - _directAsDirect_ is the proportion of direct light transmitted through the whole greenhouse surface as direct light [0;1]
 * - _directAsDiffuse_ is the proportion of direct light transmitted through the whole greenhouse surface as diffuse light [0;1]
 * - _air_ is the proportion of air transmitted through the whole greenhouse surface [0;1]
 * - _area_ is the total area of screens drawn per greenhouse surface area [m<SUP>2</SUP>/m<SUP>2</SUP>]
 *
 * Default dependencies
 * ------------
 * - a _calendar_ model with two ports:
 *   + _latitude_ [-180;180]
 *   + _azimuth_ [-90;90]
 *  - a _construction/geometry_ with a _coverArea_ port [m<SUP>2</SUP>]
 * - many (6) models of type Cover, that are children of the _greenhouse/construction_ model
 * - many (0..6) models of type Screen, that are children of the _actuators/screens_ model
 */

TransmissionBase::TransmissionBase(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(QString, directTransmissionFile, "direct_transmission_single.txt");
    InputRef(double, latitude, "calendar[latitude]");
    InputRef(double, azimuth, "calendar[azimuth]");
    InputRef(double, greenhouseSurfaceArea, "construction/geometry[coverArea]");
    InputRef(double, greenhouseGroundArea, "construction/geometry[groundArea]");
    Output(double, U);
    Output(double, netU);
    Output(double, haze);
    Output(double, diffuse);
    Output(double, directAsDirect);
    Output(double, directAsDiffuse);
    Output(double, air);
    Output(double, area);
}

void TransmissionBase::initialize() {
    dirTransTable = new DataGrid(simulation()->inputFilePath(directTransmissionFile), this);
    collectInfo();
}

namespace {
    bool isRoof(Cover::Position coverPosition) {
        return coverPosition == Cover::Roof1 ||
               coverPosition == Cover::Roof2;
    }

    QList<Screen*> matchingScreens(QList<Screen*> screens, Cover::Position coverPosition) {
        if (coverPosition == Cover::Floor)
            return QList<Screen*>();
        QMap<Cover::Position, Screen::Position> corresponding;
        corresponding[Cover::Roof1] = Screen::Roof1;
        corresponding[Cover::Roof2] = Screen::Roof2;
        corresponding[Cover::Side1] = Screen::Side1;
        corresponding[Cover::Side2] = Screen::Side2;
        corresponding[Cover::End1] = Screen::End1;
        corresponding[Cover::End2] = Screen::End2;

        QList<Screen*> matching;
        for (auto screen : screens) {
            if ( screen->pullPosition() == corresponding[coverPosition] ||
                (screen->pullPosition() == Screen::WholeRoof && isRoof(coverPosition)) ||
                (screen->pullPosition() == Screen::FlatRoof  && isRoof(coverPosition)) )
                    matching << screen;
        }
        return matching;
    }
}

void TransmissionBase::collectInfo() {
    auto covers = seekMany<Cover*>("greenhouse/construction/*");
    QList<Screen*> screens;
    if (applyScreens())
        screens = seekMany<Screen*>("actuators/screens/*");
    surfaces.clear();
    for (auto cover : covers) {
        SurfaceInfo si;
        si.area = cover->pullValue<double>("area");
        si.cover.U = cover->pullValue<double>("U");
        si.cover.diffuseTransmission = cover->pullValue<double>("diffuseTransmission");
        si.cover.haze = cover->pullValue<double>("haze");
        auto myScreens = matchingScreens(screens, cover->pullPosition());
        for (auto myScreen : myScreens) {
            SurfaceInfo::ScreenInfo sci;
            sci.energyLossReduction = myScreen->pullValue<double>("energyLossReduction");
            sci.lightTransmission = myScreen->pullValue<double>("lightTransmission");
            sci.haze = myScreen->pullValue<double>("haze");
            sci.air = myScreen->pullValue<double>("airTransmission");
            sci.state = myScreen->state();
            si.screens << sci;
        }
        surfaces << si;
    }
}

void TransmissionBase::update() {
    updateU();
    updateLightTransmission();
    updateAirTransmission();
    updateArea();
}

void TransmissionBase::updateU() {
    netU = 0;
    for (auto surface : surfaces)
        netU += surface.netU() * surface.area;
    U = netU/greenhouseSurfaceArea;
    netU /= greenhouseGroundArea;
}

double TransmissionBase::SurfaceInfo::netU() {
    double optimalTransmittance{1}, maxReduction{0};
    for (auto screen : screens) {
        Q_ASSERT(screen.state);
        double state = *screen.state;
        double reduction = screen.energyLossReduction*state;
        optimalTransmittance *= 1-reduction;
        if (reduction > maxReduction)
            maxReduction = reduction;
    }
    double optimalReduction = 1-optimalTransmittance;
    return cover.U*(1.-(optimalReduction + maxReduction)/2.);
}

void TransmissionBase::updateLightTransmission() {
    // Diffuse light
    diffuse = haze = 0.;
    for (auto surface : surfaces) {
        diffuse += surface.netLightTransmission()
                               *surface.area;
        haze += surface.netHaze()
                *surface.area;
    }
    diffuse /= greenhouseSurfaceArea;
    haze /= greenhouseSurfaceArea;
    // Direct light
    double timeAndPlace = interpolate(*dirTransTable, latitude, azimuth);
    double directTransmission = diffuse*timeAndPlace;
    directAsDirect = directTransmission*(1-haze);
    directAsDiffuse = directTransmission*haze;
}

double TransmissionBase::SurfaceInfo::netLightTransmission() {
    double transmission{1};
    transmission *= cover.diffuseTransmission;
    for (auto screen : screens)
        transmission *= 1-(*screen.state)*(1-screen.lightTransmission);
    return transmission;
}

double TransmissionBase::SurfaceInfo::netHaze() {
    double direct{1};
    direct *= 1-cover.haze;
    for (auto screen : screens)
        direct *= 1-(*screen.state)*screen.haze;
    return 1-direct;
}
void TransmissionBase::updateAirTransmission() {
    air = 0.;
    for (auto surface : surfaces)
        air += surface.netAirTransmission() * surface.area;
    air /= greenhouseSurfaceArea;
}

double TransmissionBase::SurfaceInfo::netAirTransmission() {
    double transmission{1};
    for (auto screen : screens)
        transmission *= 1-(*screen.state)*(1-screen.air);
    return transmission;
}

void TransmissionBase::updateArea() {
    area = 0.;
    for (auto surface : surfaces)
        area += surface.sumState() * surface.area;
    area /= greenhouseGroundArea;
}

double TransmissionBase::SurfaceInfo::sumState() {
    double sum{0};
    for (auto screen : screens)
        sum += *screen.state;
    return sum;
}

} //namespace

