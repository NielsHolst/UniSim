/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_SUB_STEPS_H
#define UNISIM_SUB_STEPS_H
#include <QList>
#include <usbase/model.h>


namespace UniSim{

class SubSteps : public UniSim::Model
{
public: 
    SubSteps(UniSim::Identifier name, QObject *parent=0);
    // standard methods
    void initialize();
    void update();
private:
    // parameters
    int numSubSteps;
    // links
    QList<UniSim::Component*> components;
};

} //namespace
#endif
