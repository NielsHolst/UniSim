/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef IPM_SCHEDULED_G1_LOSS_RATE_H
#define IPM_SCHEDULED_G1_LOSS_RATE_H
#include <QObject>
#include <iostream>
#include <QMessageBox>
#include <usbase/exception.h>
#include <usbase/model.h>
#include <usbase/utilities.h>

using namespace UniSim;

namespace ipm {

class ScheduledG1LossRate : public UniSim::Model
{
	//Q_OBJECT
public:
    ScheduledG1LossRate(UniSim::Identifier name, QObject *parent=0);
    void initialize();
	void reset();
    void update();
	
private:
    //parameters
	QString scheduleAsString;
	double initialValue, growthRate, value, scale, scaledValue;

    // decoded parameters;
	QList<int> daysOfYear;
	QList<int> years;
	int lenDates;

    // links
    UniSim::Model *calendar;
	
};


}
#endif
