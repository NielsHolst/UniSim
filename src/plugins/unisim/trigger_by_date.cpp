/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include "publish.h"
#include "trigger_by_date.h"

namespace UniSim{

PUBLISH(TriggerByDate)

TriggerByDate::TriggerByDate(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<QDate>("fromDate", &fromDate, QDate(1900,1,1), this,
    "The trigger will be true between @F fromDate and @F toDate at the specified @F{frequency}");

    new Parameter<double>("dateShift", &dateShift, 0., this,
    "The value of @F dateShift is added to @F {fromDate}, This is useful for adding stochasticity.");

    new Parameter<QDate>("toDate", &toDate, QDate(1900,1,1), this,
    "The trigger will be true between @F fromDate and @F toDate at the specified @F{frequency}");

    new Parameter<QString>("frequency", &frequencyAsString, "Yearly", this,
    "Frequency of trigger (@F{Daily} or @F{Yearly}: Daily or yearly in date interval");

    new Parameter<bool>("triggerAtReset", &triggerAtReset, true, this,
    "Always trigger when model is @F{reset}?");

    new Variable<bool>("value", &value, this,
    "Triggered? Updated for every time step according to the rules set by the parameters");
}

void TriggerByDate::initialize() {
    decodeFrequency();
    calendar = seekOne<Model*>("calendar");
}

void TriggerByDate::decodeFrequency() {
    QString s = frequencyAsString.toLower();
    if (s == "daily")
        frequency = DateInterval::Daily;
    else if (s == "yearly")
        frequency = DateInterval::Yearly;
    else
        throw Exception(
        QString("Frequence must take one of these values: 'Daily', or 'Once'. Not '%1'").arg(frequencyAsString),
        this);
}

void TriggerByDate::reset() {
    value = triggerAtReset;
}

void TriggerByDate::update() {
    QDate today = calendar->pullValue<QDate>("date");
//    DateInterval interval(fromDate.addDays((int) dateShift), toDate, frequency);
    DateInterval interval(fromDate, toDate, frequency);
    value = interval.includes(today);
}

} //namespace

