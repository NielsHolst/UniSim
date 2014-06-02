/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_COUNTER_H
#define UNISIM_COUNTER_H
#include <usbase/model.h>

namespace UniSim{

class Counter : public Model
{
public: 
    Counter(UniSim::Identifier name, QObject *parent=0);
    void amend();
    void initialize();
    void reset();
    void update();
    void cleanup();
    void debrief();
private:
    // Outputs
    int cAmend, cInitialize, cReset, cUpdate, cCleanup, cDebrief;

};

} //namespace
#endif
