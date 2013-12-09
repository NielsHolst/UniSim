/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <QMessageBox>
#include <usbase/exception.h>
#include <usbase/parameter.h>
#include <usbase/utilities.h>
#include "scheduled_g1_loss_rate.h"

using namespace UniSim;

namespace ipm{

ScheduledG1LossRate::ScheduledG1LossRate(Identifier name, QObject *parent)
	: Model(name,parent) 
{  
    new Parameter<QString>("scheduleAsString", &scheduleAsString, QString(), this,
    "Schedule as (dates). For example, @F {(1/1/2010 30/2/2010 17/4/2010)}.");
	new Parameter<double>("initialValue", &initialValue, 0.05, this, "Value at time zero (size or density units)");
    new Parameter<double>("growthRate", &growthRate, 1.0, this, "Exponential growth rate (size or density units per time unit");
    new Parameter<double>("scale", &scale, 1.0e3, this, "Graphical scale");
    new Variable<double>("value", &value, this, "Current value (size or density units)");
	new Variable<double>("scaledValue", &scaledValue, this, "Current value (size or density units) scaled");
}

void ScheduledG1LossRate::initialize() {

	QList<QDate> applyDates = decodeList<QDate>(scheduleAsString);
	lenDates = applyDates.size();
	
	for (int i = 0; i < lenDates; ++i){
		daysOfYear[i] = applyDates[i].dayOfYear();
		years[i] = applyDates[i].year();
	}

    calendar  = seekOne<Model*>("calendar");
}

void ScheduledG1LossRate::reset() {
    value = 0.0;
}

void ScheduledG1LossRate::update() {
    int initialYear = (calendar->pullValue<QDate>("initialDate")).year();
    double today = double(calendar->pullValue<int>("dayOfYear"));
	double t[lenDates];
	double valueiter = 0.0;	
	value = 0.0;

	for (int i = 0; i < lenDates; i++){
		t[i] = today - double(daysOfYear[i]);

		if (years[i] > initialYear)
			t[i] += (years[i] - initialYear) * 365;
		
		if (t[i] >= 0.0){
			valueiter = initialValue*exp(growthRate*t[i]);
			value = value + valueiter - value * valueiter;
			scaledValue = scale * value;
		}	
	}

}

} //namespace
