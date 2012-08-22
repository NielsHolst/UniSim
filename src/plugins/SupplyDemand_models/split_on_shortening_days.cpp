/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include "holometabola.h"
#include "split_on_shortening_days.h"

using namespace UniSim;


namespace SupplyDemand {

SplitOnShorteningDays::SplitOnShorteningDays(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("critDayLength", &critDayLength, 18., this,
    "The proportion first:second is 0:1 when day length is shorter than @F {critDayLength}. "
    "When it is longer and before summer solstice the proportion turns 1:0.");
    new Variable<double>("first", &first, this, "First proportion");
    new Variable<double>("second", &second, this, "Second proportion. The two adds up to 1.");
}

void SplitOnShorteningDays::initialize() {
    Model *calendar = seekOne<Model*>("calendar");
    dayOfYear = calendar->pullValuePtr<int>("dayOfYear");
    dayLength = calendar->pullValuePtr<double>("daylength");
}

void SplitOnShorteningDays::reset() {
    first = second = 0.5;
}

void SplitOnShorteningDays::update() {
    bool beforeSolstice = *dayOfYear < 172;
    first = beforeSolstice && *dayLength>critDayLength ? 1. : 0.;
    second = 1. - first;
}

} //namespace

