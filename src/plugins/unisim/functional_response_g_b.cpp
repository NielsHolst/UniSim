/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/utilities.h>
#include <usbase/test_num.h>
#include <usbase/variable.h>
#include "functional_response_g_b.h"

namespace UniSim{
	
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
        "Demand to cover respiration costs");

    new Variable<double>("supply", &supply, this,
        "The net supply obtained (net resource consumption, parasitoids infected [0; @F{demand}]");
    new Variable<double>("sdRatio", &sdRatio, this,
        "The supply/demand ratio [0;1]");
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
    totalDemand = (demand + respiration)/(1. - egestionRatio);
    totalSupply = GBFuncResp(totalDemand, apparency*resourceDensity);
    double netSupply = totalSupply*(1. - egestionRatio);
    egestion = totalSupply*egestionRatio;
    if (netSupply <= respiration) {
        supply = sdRatio = 0;
    }
    else {
        supply = netSupply - respiration;
        sdRatio = divBounded(supply, demand, 1.);
        TestNum::snapToZero(sdRatio);
    }
    // Update host-parasitoid variables
    attacksPerHost = resourceDensity > 0. ? supply/resourceDensity : 0.;
    propHostsAttacked = 1. - exp(-attacksPerHost);
    numHostsAttacked = resourceDensity*propHostsAttacked;
}

} //namespace

