/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_ASYMPTOTIC_DECREASING_H
#define UNISIM_ASYMPTOTIC_DECREASING_H
#include <usbase/model.h>

namespace UniSim{

class AsymptoticDecreasing : public Model
{
	//Q_OBJECT
public: 
    AsymptoticDecreasing(UniSim::Identifier name, QObject *parent=0);
    //standard methods
    void reset();
    void update();
private:
    // parameters
    double x, yMin, yMax, slope;
    // variables
    double value;
};

} //namespace
#endif
