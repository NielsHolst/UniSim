/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
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
#include <usbase/ustime.h>

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
    void updateAzimuth();

	// parameters
    double latitude, longitude;
    QDate initialDate;
    QTime initialTimeOfDay;
    int timeStep, timeStepOffset, timeZone, sample;
    char timeUnitAsChar;
    Time::Unit timeUnit;

    // variables
    QDate date;
    QTime timeOfDay;
    QDateTime dateTime;
    int day, month, year, hour, minute, second, dayOfYear, totalTime, totalTimeSteps;
    double timeStepSecs, totalDays, dateAsReal, dayLength, sinb, sinLD, cosLD, azimuth;

};

} //namespace
#endif
