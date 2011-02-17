/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef BEEHIVE_WEATHER
#define BEEHIVE_WEATHER
#include <usbase/model.h>

namespace beehive{

class Weather : public UniSim::Model
{
	Q_OBJECT
public: 
	Weather(UniSim::Identifier name, QObject *parent=0);
    // standard methods
    void initialize();
    void reset();
	void update();

private:
    // methods
    void updateTavg();

    // pull variables
    double Tavg;

    // links
    UniSim::Model *records;
};

} //namespace
#endif
