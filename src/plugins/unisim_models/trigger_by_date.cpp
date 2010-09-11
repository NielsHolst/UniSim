/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include "trigger_by_date.h"

namespace UniSim{

TriggerByDate::TriggerByDate(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<QDate>("byDate", &byDate, QDate(1900,1,1), this,
    "Triggering will happen from this date and afterwards at the specified @F{frequency}");

    new Parameter<QString>("frequency", &frequencyAsString, "Daily", this,
    "Frequency of trigger (@F{Daily}, @F{Monthly}, @F{Yearly} or @F{Once})");

    new Parameter<bool>("triggerAtReset", &triggerAtReset, true, this,
    "Always trigger when model is @F{reset}?");

    new PullVariable<bool>("triggered", &triggered, this,
    "Updated for every time step according to the rules set by the parameters");
}

void TriggerByDate::initialize() {
    decodeFrequency();
    calendar = seekOne<Model*>("calendar");
}

void TriggerByDate::decodeFrequency() {
    QString s = frequencyAsString.toLower();
    if (s == "daily")
        frequency = Daily;
    else if (s == "monthly")
        frequency = Monthly;
    else if (s == "yearly")
        frequency = Yearly;
    else if (s == "once")
        frequency = Once;
    else
        throw Exception(
        "Frequence must take one of these values: 'Daily', 'Monthly', 'Yearly' or 'Once'. Not '" +
        frequencyAsString + "'", this);
}

void TriggerByDate::reset() {
    triggered = triggerAtReset;
}

void TriggerByDate::update() {
    QDate today = calendar->pullVariable<QDate>("date");
    if (today < byDate)
        triggered = false;
    else {
        bool dayMatch = today.day() == byDate.day();
        bool monthMatch = today.month() == byDate.month();
        bool dateMatch = today == byDate;
        switch (frequency) {
        case Daily:
            triggered = true;
            break;
        case Monthly:
            triggered = dayMatch;
            break;
        case Yearly:
            triggered = dayMatch && monthMatch;
            break;
        case Once:
            triggered = dateMatch;
            break;
        }
    }
}

} //namespace

