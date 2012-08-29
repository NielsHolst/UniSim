/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_EXPONENTIAL
#define UNISIM_EXPONENTIAL

#include <usbase/model.h>

namespace UniSim{

class Exponential : public Model
{
	//Q_OBJECT
public:
    Exponential(Identifier name, QObject *parent=0);
    //standard methods
    void initialize();
    void reset();
    void update();

private:
    // parameters
    double initialValue, growthRate;

    // state
    double value, increment;

    // links
    UniSim::Model *time;
};

} //namespace


#endif
