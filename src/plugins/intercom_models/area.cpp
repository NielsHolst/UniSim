/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <QMessageBox>
#include <usbase/exception.h>
#include <usbase/pull_variable.h>
#include <usbase/utilities.h>
#include "../unisim_models/calendar.h"
#include "area.h"
#include "constants.h"
#include "plant.h"
#include "weather.h"

using namespace UniSim;

namespace intercom{

Area::Area(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    new PullVariable<double>("lai", &lai, this, "description");

    lookupDist["Symmetric"] = Symmetric;
    lookupDist["Even"] = Even;
    lookupDist["Tapering"] = Tapering;
    lookupDist["TopHeavy"] = TopHeavy;
}

void Area::initialize() {
    setParameter("initial", &initial, 0.01, "description");
    setParameter("distribution", &distText, QString("Symmetric"), "description");
    setParameter("scatteringCoeff", &scatteringCoeff, 0.2, "description");
    setParameter("kDiffuse", &kDiffuse, 0.7, "description");

    calendar = seekOne<Model*>("calendar");
    weather = seekOne<Model*>("weather");
    plant = seekOneAscendant<Plant*>("*");
    plantHeight = plant->seekOneDescendant<Model*>("height");
    lightUseEfficiency = seekOneChild<Model*>("lightUseEfficiency");
    assimilationMax = seekOneChild<Model*>("amax");
}

void Area::reset() {
    lai = initial;
}

Area::Distribution Area::distribution() const {
    if (!lookupDist.contains(distText))
        throw Exception("Unknown distribution type: " + distText);
    return lookupDist.value(distText);
}

LightComponents Area::calcEffectiveAreaAbove(double height) {
    LightComponents k = calc_k();

    LightComponents eaa;
    double areaAbove = aboveHeight(height);
    for (int lc = 0; lc < 3; ++lc) {
        eaa[lc] = k[lc]*areaAbove;
    }

    return eaa;
}

LightComponents Area::calc_k() const {
    double scat = sqrt(1 - scatteringCoeff);
    double sinb = calendar->pullVariable<double>("sinb");
    Q_ASSERT_X(sinb > 0., "Area::calc_k()",
               qPrintable("sinb = " + QString::number(sinb)));
    LightComponents k;
    k[Diffuse] = kDiffuse;
    k[DirectDirect] = 0.5/sinb*kDiffuse/0.8/scat;
    k[DirectTotal] = k[DirectDirect]*scat;
    return k;
}

PhotosyntheticRate Area::calcPhotosynthesis(LightComponents eaa) {
    PhotosyntheticRate psInShade = calcPhotosynthesisInShade(eaa);
    PhotosyntheticRate psInSun = calcPhotosynthesisInSun(psInShade);
    PhotosyntheticRate psTotal = calcPhotosynthesisTotal(eaa, psInShade, psInSun);
    cout << " Area::calcPhotosynthesis() psTotal = " << psTotal.absorption() << " " << psTotal.assimilation() << "\n";
    return psTotal;
}

PhotosyntheticRate Area::calcPhotosynthesisInShade(LightComponents eaa) {
    LightComponents absorbedComp = calcAbsorptionInShade(eaa);
    double absorption = netAbsorption(absorbedComp);
    double assimilation = assimilationRate(absorption);
    return PhotosyntheticRate(absorption, assimilation);
}

LightComponents Area::calcAbsorptionInShade(LightComponents eaa) {
    double sinb = calendar->pullVariable<double>("sinb");
    if (sinb == 0.)
        return LightComponents();

    double refHorz = (1 - sqrt(0.8))/(1 + sqrt(0.8));
    double refSphec = refHorz*2./(1. + 1.6*sinb);

    LightComponents k = calc_k();

    LightComponents par;
    par[Diffuse] = weather->pullVariable<double>("parDiffuse");
    par[DirectDirect] = weather->pullVariable<double>("parDirect");
    par[DirectTotal] = weather->pullVariable<double>("parDirect");

    LightComponents reflected;
    reflected[Diffuse] = refHorz;
    reflected[DirectDirect] = scatteringCoeff;
    reflected[DirectTotal] = refSphec;

    LightComponents absorbed;
    for (int lc = 0; lc < 3; ++lc)
        absorbed[lc] = k[lc] * (1. - reflected[lc]) * par[lc] * exp(-eaa.value(lc));

    return absorbed;
}

double Area::netAbsorption(const LightComponents &absorbed) const {
    double netAbsorbed = absorbed.value(Diffuse) + absorbed.value(DirectTotal)
                         - absorbed.value(DirectDirect);
    if (netAbsorbed < 0) {
        netAbsorbed = 0.;
     /*   if (netAbsorbed > -0.1)
            netAbsorbed = 0.;
        else
            throw Exception("Shaded absorption (" + QString::number(netAbsorbed) + ")" +
                            " < 0 in " + const_cast<Area*>(this)->fullName());
     */
    }
    return netAbsorbed;
}

double Area::assimilationRate(double absorption) const {
    double efficiency = lightUseEfficiency->pullVariable<double>("efficiency");
    double amax = assimilationMax->pullVariable<double>("amax");
    return amax == 0. ? 0. : amax*(1. - exp(-absorption*efficiency/amax));
}

PhotosyntheticRate Area::calcPhotosynthesisInSun(PhotosyntheticRate psInShade) {
    double sinb = calendar->pullVariable<double>("sinb");
    double parDirect = weather->pullVariable<double>("parDirect");
    double perpendicular = (1. - scatteringCoeff)*parDirect/sinb;

    double absorbed = 0;
    double assimilated = 0.;
    // integrate over leaf angles
    for (int i = 0; i<3; i++) {
        double abso = psInShade.absorption() + perpendicular*XGAUSS3[i];
        double assi = assimilationRate(abso);
        absorbed += abso*WGAUSS3[i];
        assimilated += assi*WGAUSS3[i];
    }
    return PhotosyntheticRate(absorbed, assimilated);
}

PhotosyntheticRate Area::calcPhotosynthesisTotal(LightComponents eaa, PhotosyntheticRate psInShade, PhotosyntheticRate psInSun) {
    double sunlit = exp(-eaa.value(DirectDirect));
    double absorbed = sunlit*psInSun.absorption() + (1 - sunlit)*psInShade.absorption();
    double assimilated = sunlit*psInSun.assimilation() + (1 - sunlit)*psInShade.assimilation();
    // Finally, convert J/m2/s to MJ/m2/d
    return PhotosyntheticRate(/*3600.*1e-6*/absorbed, assimilated);
}

double Area::atHeight(double height) const {
    double ph = plantHeight->pullVariable<double>("height");
    if (ph == 0. || height > ph)
        return 0.;

    double area;
    switch (distribution()) {
        case Symmetric:
            area = lai*6./pow(ph,3)*height*(ph-height);
            break;
        case Even:
            area = lai/ph;
            break;
        case Tapering:
            area = lai*((-0.2*height/ph + 0.2)/0.1)/ph;
            break;
        case TopHeavy:
            area = lai*30.*pow(height,4)/pow(ph,5)*(1. - height/ph);
            break;
    }
    return area;
}

double Area::aboveHeight(double height) const {
    double ph = plantHeight->pullVariable<double>("height");
    if (ph == 0. || height > ph)
        return 0.;

    double area;
    switch (distribution()) {
        case Symmetric:
            area = lai - lai/pow(ph,3)*height*height*(3.*ph - 2.*height);
            break;
        case Even:
            area = lai*(ph - height)/ph;
            break;
        case Tapering:
            area = lai*(1-(-(pow((height/ph),2)) + 2.*(height/ph)));
            break;
        case TopHeavy:
            area = lai*(-.164 + 1.172/(1. + exp(8.352*(height/ph - .759))));
            break;
    }
    return area;
}


} //namespace

