/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_TIME_LIMITED_H
#define UNISIM_TIME_LIMITED_H

#include <usbase/integrator.h>

namespace UniSim{
	
class Model;

class TimeLimited : public Integrator
{
	//Q_OBJECT

public:
    TimeLimited(Identifier name, QObject *parent=0);

    // standard methods
    void initialize();

    // special methods
    bool doNextStep();

private:
    // Parameters
    double maxTime;

    // Links
    Model* time;
};


} //namespace


#endif
