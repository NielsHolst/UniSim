/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef IPM_LOGANB_TIME
#define IPM_LOGANB_TIME
#include <QObject>
#include <usbase/model.h>
#include "../unisim/physiological_time.h"

using namespace UniSim;

namespace ipm{

class LoganBTime : public PhysiologicalTime
{
	//Q_OBJECT
public: 
    LoganBTime(UniSim::Identifier name, QObject *parent=0);
	//standard methods
    virtual void initialize();

    // special methods
    virtual double calcDailyTimeStep();

private:
	// parameters
    double a, b, c, d, e;

	// state
    double step, total;

    // models
    UniSim::Model *weather;
};

} //namespace
#endif
