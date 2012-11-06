/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_RANDOM_BASE
#define UNISIM_RANDOM_BASE
#include <usbase/identifier.h>
#include <usbase/model.h>

namespace UniSim{

class RandomBase : public Model
{
	//Q_OBJECT
public: 
    RandomBase(Identifier name, QObject *parent=0);
    // standard methods
    virtual void initialize();
    virtual void reset();
    virtual void update();
private:
    // methods
    virtual double drawValue() = 0;
    bool triggered();
    void nextValue();
    // parameters
    double minValue, maxValue;
    int maxTries;
    // pull variables
    double value;
    // links
    Model *trigger;
};

} //namespace
#endif
