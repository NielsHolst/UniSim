/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QMapIterator>
#include <usbase/data_grid.h>
#include <usbase/interpolate.h>
#include <usengine/simulation.h>
#include "construction_cover.h"
#include "control_element.h"
#include "screen.h"
#include "surface.h"

using namespace UniSim;

namespace vg {
	
Surface::Surface(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameter<QString>(Name(directTransmissionFile), QString("direct_transmission_single.txt"),
                         "Table with transmission values [0;1] according to latitude (rows) and sun azimuth (columns)");
    addParameterRef<double>(Name(latitude), "calendar[latitude]");
    addParameterRef<double>(Name(azimuth), "calendar[azimuth]");
    addVariable<double>(Name(K), "Common K of surface");
    addVariable<double>(Name(diffuseTransmission), "Diffuse light transmitted through surface [0;1]");
    addVariable<double>(Name(directTransmissionAsDirect), "Direct light transmitted through surface as direct light[0;1]");
    addVariable<double>(Name(directTransmissionAsDiffuse), "Direct light transmitted through surface as diffusse light[0;1]");
}

void Surface::amend() {
    dirTransTable = new DataGrid(simulation()->inputFilePath(directTransmissionFile), this);
    collectInfo();
    updateTotalArea();
}

void Surface::update() {
    updateK();
    updateLightTransmission();
}

namespace {
    bool isRoof(ConstructionCover::Position coverPosition) {
        return coverPosition == ConstructionCover::Roof1 || ConstructionCover::Roof2;
    }

    QList<Screen*> matchingScreens(QList<Screen*> screens, ConstructionCover::Position coverPosition) {
        QMap<ConstructionCover::Position, Screen::Position> corresponding;
        corresponding[ConstructionCover::Roof1] = Screen::Roof1;
        corresponding[ConstructionCover::Roof2] = Screen::Roof2;
        corresponding[ConstructionCover::Side1] = Screen::Side1;
        corresponding[ConstructionCover::Side2] = Screen::Side2;
        corresponding[ConstructionCover::End1] = Screen::End1;
        corresponding[ConstructionCover::End2] = Screen::End2;
        QList<Screen*> matching;
        for (auto screen : screens) {
            if ( screen->pullPosition() == corresponding[coverPosition] ||
                (screen->pullPosition() == Screen::WholeRoof && isRoof(coverPosition)) ||
                (screen->pullPosition() == Screen::FlatRoof  && isRoof(coverPosition)) )
                matching << screen;
        }
        return matching;
    }

    void checkK(double K, Model *context) {
        if (K <= 0.) {
            QString msg = "K-value (%1) must be positive";
            throw Exception(msg.arg(K), context);
        }

    }
}

void Surface::collectInfo() {
    auto covers = seekMany<ConstructionCover*>("greenhouse/construction/*");
    auto screens = seekMany<Screen*>("effectuators/screens/*");
    for (auto cover : covers) {
        SurfaceInfo si;
        si.area = cover->pullValue<double>("area");
        si.cover.K = cover->pullValue<double>("K");
        checkK(si.cover.K, cover);
        si.cover.diffuseTransmission = cover->pullValue<double>("diffuseTransmission");
        si.cover.directTransmissionAtNoon = cover->pullValue<double>("directTransmissionAtNoon");
        si.cover.haze = cover->pullValue<double>("haze");
        auto myScreens = matchingScreens(screens, cover->pullPosition());
        for (auto myScreen : myScreens) {
            SurfaceInfo::ScreenInfo sci;
            sci.K = myScreen->pullValue<double>("K");
            checkK(sci.K, myScreen);
            sci.diffuseTransmission = myScreen->pullValue<double>("diffuseTransmission");
            sci.haze = myScreen->pullValue<double>("haze");
            auto control = myScreen->seekOneChild<ControlElement*>("*");
            sci.state = control->pullValuePtr<double>("state");
            si.screens << sci;
        }
        surfaces << si;
    }
}

void Surface::updateTotalArea() {
    totalArea = 0;
    for (auto surface : surfaces) {
        totalArea += surface.area;
    }
}

//! Compute total K as a combination of cover and relevant screens
/*! For each surface, transform K-values to resistances scaling by screen state, accumulate them and find the surface K
 *  by back-transforming from resistance to K.  Weigh each surface K by surface area to find the area-weighted average K.
 */
void Surface::updateK() {
    K = 0;
    for (auto surface : surfaces) {
        double conductance = 1./surface.cover.K;
        for (auto screen : surface.screens) {
            double openness = 1.-*screen.state;
            conductance += 1./screen.K*openness;
        }
        K += 1./conductance*surface.area;
    }
    K = K/totalArea;
}

//! Compute overall diffuse and direct light transmission as a combination of cover and relevant screens
/*! For diffuse light:
 *
 *  Find the product of all transmission for each surface, scaling by pertinent screen state.
 *  Weigh each surface transmission by surface area to find the area-weighted average transmission.
 *
 *  For direct light:
 *
 *  The transmission through the cover is calculated by linear interpolation from a table indexed by latitude and sun azimuth.
 *  The table value [0;1] is scaled by the \em directTransmissionAtNoon of the cover.
 *  Assume that subsequent the transmission through screens is the same as for diffuse light.
 *  The direct component of direct light, i.e. the component [0;1] remaining direct after passage through the material (cover or screen),
 *  is computed for all surfaces.
 *  Weigh for each surface, both directTransmission and direct component, to find the area-weighted averages.
 */
void Surface::updateLightTransmission() {
    double directComponent{0}, directTransmission{0};
    diffuseTransmission = 0.;
    for (auto surface : surfaces) {
        double screenTransmission = 1.;
        double directComponent = 1.-surface.cover.haze;
        for (auto screen : surface.screens) {
            double openness = 1.-*screen.state;
            screenTransmission *= screen.diffuseTransmission*openness;
            directComponent *= (1.- screen.haze)*openness;
        }
        diffuseTransmission += surface.cover.diffuseTransmission * screenTransmission * surface.area;
        directTransmission += surface.cover.directTransmissionAtNoon * screenTransmission * surface.area;
        directComponent += directComponent*surface.area;
    }
    diffuseTransmission /= totalArea;
    directTransmission /= totalArea;
    directComponent /= totalArea;
    double timeAndPlace = interpolate(*dirTransTable, latitude, azimuth);
    directTransmission *= timeAndPlace;
    directTransmissionAsDirect = directTransmission*directComponent;
    directTransmissionAsDiffuse = directTransmission*(1.-directComponent);
}



} //namespace

