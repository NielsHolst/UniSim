/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
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
public: 
    PhysiologicalTime(UniSim::Identifier name, QObject *parent=0);
    // standard methods
    void initialize();
    void reset();
    void update();

    // special methods
    virtual double calcDailyTimeStep() = 0;

private:
    bool doReset, isTicking;
    double calendarTimeStep;
    double step, total;
    // Links
    UniSim::Model *calendar;
    const bool *triggered;
};

} //namespace
#endif
