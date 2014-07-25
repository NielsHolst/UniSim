/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/string_conversion.h>
#include <usbase/ustime.h>
#include "publish.h"
#include "steps_time_interval.h"

namespace UniSim{

PUBLISH(StepsTimeInterval)

StepsTimeInterval::StepsTimeInterval(Identifier name, QObject *parent)
    : Integrator(name, parent)
{
    Input(QDate, beginDate, QDate());
    Input(QDate, endDate, QDate());
    Input(QTime, beginTime, QTime());
    Input(QTime, endTime, QTime());
    Input(int, timeStep, 1);
    Input2(char, timeUnitAsChar, timeUnit, 'd');
    Output(int, maxSteps);
}

void StepsTimeInterval::reset() {
    Integrator::reset();
    maxSteps = secsInterval() / secsTimeStep();
    if (maxSteps < 1) maxSteps = 1;
}

long long int StepsTimeInterval::secsInterval() {
    QDateTime begin(beginDate, beginTime);
    QDateTime end(endDate, endTime);
    return begin.secsTo(end);
}

long long int StepsTimeInterval::secsTimeStep() {
    Time::Unit timeUnit = Time::charToUnit(timeUnitAsChar, this);
    return timeStep*Time::conversionFactor(timeUnit, Time::Seconds);
}


bool StepsTimeInterval::doNextStep()
{
    ++stepNumber;
	progress = double(stepNumber)/maxSteps;
    return stepNumber <= maxSteps;
}

} //namespace

