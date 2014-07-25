/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_STEPS_TIME_INTERVAL_H
#define UNISIM_STEPS_TIME_INTERVAL_H

#include <QDate>
#include <QTime>
#include <usbase/integrator.h>

namespace UniSim{
	
class Model;

class StepsTimeInterval : public Integrator
{
public:
    StepsTimeInterval(Identifier name, QObject *parent=0);
    void reset();

    // special methods
    bool doNextStep();
	
private:
    // Inputs
    QDate beginDate, endDate;
    QTime beginTime, endTime;
    int timeStep;
    char timeUnitAsChar;
    // Data
    int maxSteps;
    // Methods
    long long int secsInterval();
    long long int secsTimeStep();
};

} //namespace

#endif
