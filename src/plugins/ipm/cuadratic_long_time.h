/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_CUADRATIC_LONG_TIME
#define UNISIM_CUADRATIC_LOG_TIME
#include <QObject>
#include <usbase/model.h>
#include "../unisim/physiological_time.h"

using namespace UniSim;

namespace ipm{

class CuadraticLongTime : public PhysiologicalTime
{
	//Q_OBJECT
public: 
    CuadraticLongTime(UniSim::Identifier name, QObject *parent=0);
	//standard methods
    virtual void initialize();

    // special methods
    virtual double calcDailyTimeStep();

private:
	// parameters
    double a, b, c, d;

	// state
    double step, total;

    // models
    UniSim::Model *weather;
};

} //namespace
#endif
