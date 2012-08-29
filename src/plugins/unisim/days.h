/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_DAY_S
#define UNISIM_DAY_S
#include <QObject>
#include <usbase/model.h>
#include "physiological_time.h"


namespace UniSim{

class Days : public PhysiologicalTime
{
	//Q_OBJECT
public: 
    Days(UniSim::Identifier name, QObject *parent=0);
    // special methods
    virtual double calcDailyTimeStep();
};

} //namespace
#endif
