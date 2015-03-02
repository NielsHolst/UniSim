/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/utilities.h>
#include <usbase/test_num.h>
#include <usbase/variable.h>
#include "publish.h"
#include "functional_response_g_b.h"

namespace UniSim{

PUBLISH(FunctionalResponseGB)

FunctionalResponseGB::FunctionalResponseGB(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("apparency", &apparency, 1., this,
        "The apparancy of the resource, also known as attack rate or search rate");
    new Parameter<double>("apparancy", &apparency, 1., this,
        "Common miss-spelling");
    new Parameter<double>("resourceDensity", &resourceDensity, 100., this,
        "The density of the resource (prey, host)");
    new Parameter<double>("demand", &demand, 1.0, this,
        "The finite demand rate of the consumer (predator, parasitoid) population");
    new Parameter<double>("egestionRatio", &egestionRatio, 0., this,
        "Egestion fraction of consumption");
    new Parameter<double>("respiration", &respiration, 0., this,
        "The finite demand rate to cover respiration costs");
    new Parameter<double>("conversionCost", &conversionCost, 0., this,
        "Proportion of the consumption going to conversion cost for converting prey to predator mass");
    new Parameter<bool>("costOnRespiration", &costOnRespiration, true, this,
        "Apply conversion cost also on respiration?");
    new Parameter<bool>("truncateSupply", &truncateSupply, false, this,
        "If supply gets negative (i.e. too little is consumed to cover respiration) should it be truncated to zero?");

    new Variable<double>("supply", &supply, this,
        "The net supply obtained (net resource consumption, parasitoids infected). "
        "This will always be <= demand. If respiration exceeds consumption and truncateSupply is false, then supply will be negative.");
    new Variable<double>("sdRatio", &sdRatio, this,
        "The supply/demand ratio [0;1]. This will be zero if supply is negative");
    new Variable<double>("totalDemand", &totalDemand, this,
        "Total demand obtained to cover net supply, egestion and respiration");
    new Variable<double>("totalSupply", &totalSupply, this,
        "Total supply obtained to cover net supply, egestion and respiration");
    new Variable<double>("egestion", &egestion, this,
        "Amount of @F totalSupply lost to egestion");
    new Variable<double>("attacksPerHost", &attacksPerHost, this,
        "Amount of @F totalSupply lost to egestion");
    new Variable<double>("numHostsAttacked", &numHostsAttacked, this,
        "Amount of @F totalSupply lost to egestion");
    new Variable<double>("propHostsAttacked", &propHostsAttacked, this,
        "Amount of @F totalSupply lost to egestion");
}

void FunctionalResponseGB::reset() {
    supply = sdRatio = totalDemand = totalSupply = egestion =
    attacksPerHost = numHostsAttacked = propHostsAttacked = 0;
}

void FunctionalResponseGB::update() {
    if (egestionRatio < 0. || egestionRatio >= 1.) {
        QString msg("Illegal value for egestionRatio (%1)");
        throw Exception(msg.arg(egestionRatio));
    }
    // Compute total supply (i.e., total kill)
    totalDemand = costOnRespiration ?
                (demand + respiration)/(1-conversionCost) :
                demand/(1-conversionCost) + respiration;
    totalDemand /= (1. - egestionRatio);
    totalSupply = GBFuncResp(totalDemand, apparency*resourceDensity);
    // Allocate total supply
    supply = totalSupply;
    egestion = supply*egestionRatio;
    supply -= egestion;
    supply -= costOnRespiration ? respiration/(1-conversionCost) : respiration;
    if (supply < 0.) {
        sdRatio = 0.;
        if (truncateSupply) supply = 0.;
    }
    else {
        sdRatio = divBounded(supply, demand, 1.);
        TestNum::snapToZero(sdRatio);
    }
    // Update host-parasitoid variables
    if (supply > 0) {
        attacksPerHost = resourceDensity > 0. ? supply/resourceDensity : 0.;
        propHostsAttacked = 1. - exp(-attacksPerHost);
        numHostsAttacked = resourceDensity*propHostsAttacked;
    }
    else {
        attacksPerHost =
        propHostsAttacked =
        numHostsAttacked = 0.;
    }
}

} //namespace

