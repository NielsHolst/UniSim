/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include <usbase/utilities.h>
#include "../standard_models/calendar.h"
#include "area.h"
#include "community.h"
#include "constants.h"
#include "plant.h"
#include "weather.h"

using namespace UniSim;

namespace intercom{

Area::Area(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    setState("lai", &lai);
    setState("absorption", &absorption.total);
    setState("assimilation", &assimilation.total);
    setState("assimEfficiency", &assimEfficiency);

    lookupDist["Symmetric"] = Symmetric;
    lookupDist["Even"] = Even;
    lookupDist["Tapering"] = Tapering;
    lookupDist["TopHeavy"] = TopHeavy;
}

void Area::initialize() {
    setParameter("distribution", &distText, QString("Symmetric"));
    setParameter("scatteringCoeff", &scatteringCoeff, 0.2);
    setParameter("kDiffuse", &k[Diffuse], 0.7);

    if (!lookupDist.contains(distText))
        throw Exception("Unknown distribution type: " + distText);
    distribution = lookupDist.value(distText);

    calendar = findOne<UniSim::Calendar*>("calendar");
    weather = findOne<Weather*>("weather");
    plant = findAscendant<Plant*>("*");
    plantHeight = plant->findChild<Model*>("height");
    assimilationMax = findChild<Model*>("amax");
    Community *community = findAscendant<Community*>("*");
    cs = community->statePtr();
}

void Area::reset() {
    lai = 0.;
}

void Area::update() {
    amax = assimilationMax->state("amax");
    double Tday = weather->state("Tday");
    assimEfficiency = (-0.0095*Tday + 0.635)/10.;

    updateLightUseInShade();
    updateLightUseInSun();
    updateLightUseTotal();
}

AbsorptionExponents Area::absorptionExponents(double layerHeight_) {
    layerHeight = layerHeight_;
    double scat = sqrt(1 - scatteringCoeff);
    k[Direct] = 0.5/cs->sinb*k[Diffuse]/0.8/scat;
    k[DirectTotal] = k[Direct]*scat;

    AbsorptionExponents exponents;
    for (int i = 0; i < NumLightComponents; ++i) {
        double areaAbove = aboveHeight(layerHeight);
        exponents[i] = k[i]*areaAbove;
    }

    return exponents;
}

void Area::updateLightUseInShade()
{
    double refh = (1 - sqrt(0.8))/(1 + sqrt(0.8));
    double refs = refh*2./(1. + 1.6*cs->sinb);

    double absorbed[NumLightComponents], par[NumLightComponents];
    par[Diffuse] = cs->par.diffuse;
    par[Direct] = cs->par.direct;
    par[DirectTotal] = cs->par.direct;
    for (int lc = 0; lc < NumLightComponents; ++lc)
        absorbed[lc] = (1. - refs) * par[lc] * k[lc] * exp(-cs->absorptionExponents.value(lc));

    absorption.inShade = absorbed[Diffuse] + absorbed[DirectTotal] - absorbed[Direct];
    if (absorption.inShade < 0) {
        if (absorption.inShade > -1e-4)
            absorption.inShade = 0.;
        else
            throw Exception("Shaded absorption (" + QString::number(absorption.inShade) + ")" +
                            " < 0 in area of " + plant->objectName());
    }
    assimilation.inShade =
        amax == 0. ? 0. :
        amax*(1. - exp(-absorption.inShade*assimEfficiency/amax));
}

void Area::updateLightUseInSun()
{
    double perpendicular = (1. - 0.2)*cs->par.direct/cs->sinb;
    absorption.inSun = 0;
    assimilation.inSun = 0.;
    for (int i = 0; i<3; i++)
    {
        double absorp = absorption.inShade + perpendicular*Xgauss2[i];
        double assim = amax*(1. - exp(-absorp*assimEfficiency/amax));
        absorption.inSun += absorp*Wgauss2[i];
        assimilation.inSun += assim*Wgauss2[i];
    }

}

void Area::updateLightUseTotal()
{
    double leafDensity = atHeight(layerHeight);
    double sunlitFraction = exp(-cs->absorptionExponents.value(Direct));
    assimilation.total = (sunlitFraction*assimilation.inSun +
                        (1 - sunlitFraction)*assimilation.inShade)*leafDensity;
    absorption.total = (sunlitFraction*absorption.inSun +
                       (1 - sunlitFraction)*absorption.inShade)*leafDensity;
}

double Area::atHeight(double height) {
    double ph = plantHeight->state("height");
    if (ph == 0. || height > ph)
        return 0.;

    double area;
    switch (distribution) {
        case Symmetric:
            area = lai*6./pow(ph,3)*ph*(ph-height);
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

double Area::aboveHeight(double height) {
    double ph = plantHeight->state("height");
    if (ph == 0. || height > ph)
        return 0.;

    double area;
    switch (distribution) {
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

