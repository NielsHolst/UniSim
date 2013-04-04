/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include <usbase/utilities.h>
#include "pollen_loss_sampled.h"

using namespace UniSim;


namespace ecotox {

PollenLossSampled::PollenLossSampled(UniSim::Identifier name, QObject *parent)
    : Model(name, parent), distribution(0), variate(0)
{
    new Parameter<QString>("rates", &ratesAsString, QString("(0 0.5 0.9)"), this,
        "List of daily loss rates from which one is drawn at random every day. "
        "A value of @F{(0 0.5 0.9)}, for example, results in a daily loss rate of either 0, 50% "
        "or 90% with equal probability");
    new Variable<double>("value", &value, this,
                             "Pollen survival rate (per day)");
}

PollenLossSampled::~PollenLossSampled() {
    delete distribution;
    delete variate;
}

void PollenLossSampled::initialize() {
    rates = decodeList<double>(ratesAsString, this);
    int n = rates.size();
    distribution = new Distribution(0, n-1);
    variate = new Variate(*randomGenerator(), *distribution);
}

void PollenLossSampled::reset() {
    update();
}

void PollenLossSampled::update() {
    int ix = (*variate)();
    value = rates.value(ix);
}


} //namespace

