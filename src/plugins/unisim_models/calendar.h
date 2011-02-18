/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_CALENDAR
#define UNISIM_CALENDAR
#include <QDate>
#include <QDateTime>
#include <QTime>
#include <usbase/model.h>
#include <usbase/time.h>

namespace UniSim{

class Calendar : public UniSim::Model
{
	Q_OBJECT
public: 
	Calendar(UniSim::Identifier name, QObject *parent=0);
	//standard methods
	void initialize();
	void reset();
	void update();

private slots:
    void handleClockTick(double hour);

private:
    // methods
    void updateDerived();

	// parameters
    double latitude;
    QDate initialDate;
    QTime initialTimeOfDay;
    int timeStep, timeStepOffset;
    char timeUnitAsChar;
    Time::Unit timeUnit;

    // pull variables
    QDate date;
    QTime timeOfDay;
    QDateTime dateTime;
    int day, month, year, hour, minute, second, dayOfYear, totalTime, totalTimeSteps;
    double totalDays, dateAsReal, dayLength, sinb, sinLD, cosLD;

};

} //namespace
#endif
