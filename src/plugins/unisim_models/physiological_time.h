/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_PHYSIOLOGICAL_TIME
#define UNISIM_PHYSIOLOGICAL_TIME
#include <QObject>
#include <usbase/model.h>

namespace UniSim{

class PhysiologicalTime : public Model
{
	Q_OBJECT
public: 
    PhysiologicalTime(UniSim::Identifier name, QObject *parent=0);
    // standard methods
    virtual void initialize();
    virtual void reset();
    void update();

    // special methods
    virtual double calcDailyTimeStep() = 0;

private:
    // pull variables
    double step, total;
    // links
    UniSim::Model *calendar;
    // state
    double calendarTimeStep;
};

} //namespace
#endif
