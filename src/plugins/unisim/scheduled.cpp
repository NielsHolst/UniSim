/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/decode_list.h>
#include <usbase/exception.h>
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include "publish.h"
#include "scheduled.h"

namespace UniSim{

PUBLISH(Scheduled)

Scheduled::Scheduled(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<QString>("schedule", &scheduleAsString, QString(), this,
    "Schedule as (time value) pairs. For example, @F {((0 10)(20.5 30)(65 48.6))}.");

    new Variable<double>("value", &value, this, "Current value");
    new Variable<double>("increment", &increment, this, "Increment in value during the latest time step");
}

void Scheduled::initialize() {
    time = seekOneNearest<Model*>("time");
    schedule = decodeList<double,double>(scheduleAsString);
    if (schedule.isEmpty())
        throw Exception("Schedule is empty", this);
}

void Scheduled::reset() {
    QMapIterator<double, double> sc(schedule);
    sc.next();
    value = sc.value();
    increment = 0;
}

void Scheduled::update() {
    double t = time->pullValue<double>("total");
    double valueBefore = value;
    value = interpolateValue(t);
    increment = value - valueBefore;
}

double Scheduled::interpolateValue(double x) {
    double x0, x1, y0, y1;
    QMapIterator<double, double> sc(schedule);
    sc.next();
    x1 = sc.key();
    y1 = sc.value();
    if (x < x1)
        return y1;

    while (sc.hasNext() && x1 < x) {
        sc.next();
        x0 = x1;
        y0 = y1;
        x1 = sc.key();
        y1 = sc.value();
    }

    if (x1 <= x)
        return y1;
    return y0 + (x-x0)*(y1-y0)/(x1-x0);
}



} //namespace

