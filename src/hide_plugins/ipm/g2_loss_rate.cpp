/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QMessageBox>
#include <usbase/exception.h>
#include <usbase/parameter.h>
#include <usbase/utilities.h>
#include "g2_loss_rate.h"

using namespace UniSim;

namespace ipm{

G2LossRate::G2LossRate(Identifier name, QObject *parent)
	: Model(name,parent) 
{ 
    new Parameter<int>("day", &day, 1, this, "Day in month of imposed loss rate");
    new Parameter<int>("month", &month, 1, this, "Month of imposed loss rate");
	new Parameter<int>("year", &year, 2010, this, "Year of imposed loss rate");
    new Parameter<double>("initialValue", &initialValue, 0.05, this, "Value at time zero (size or density units)");
    new Parameter<double>("growthRate", &growthRate, 1.0, this, "Exponential growth rate (size or density units per time unit");
    new Parameter<double>("predatorDensity", &predatorDensity, 0.0, this, "Predator population density");
    new Parameter<double>("scale", &scale, 1.0e3, this, "Graphical scale");
    new Variable<double>("value", &value, this, "Current value (size or density units)");
	new Variable<double>("scaledValue", &scaledValue, this, "Current value (size or density units) scaled");
}

void G2LossRate::initialize() {
    dayOfYear = UniSim::toDayOfYear(day, month);
    calendar  = seekOne<Model*>("calendar");
}

void G2LossRate::reset() {
    value = initialValue;
}

void G2LossRate::update() {
    int initialYear = (calendar->pullValue<QDate>("initialDate")).year();
    double today = double(calendar->pullValue<int>("dayOfYear"));
	double t = today - double(dayOfYear);
	if (year > initialYear)
		t += (year - initialYear) * 365;
	value = 0.0;
	
	if (t >= 0.0){
		value = initialValue*exp(growthRate*t)*predatorDensity;
		scaledValue = scale * value;
	}
}

} //namespace