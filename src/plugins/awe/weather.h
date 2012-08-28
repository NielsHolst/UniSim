/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef AWE_WEATHER_H
#define AWE_WEATHER_H

#include <QObject>
#include <usbase/model.h>

namespace awe {

class Weather : public UniSim::Model
{
	Q_OBJECT
public:
    Weather(UniSim::Identifier name, QObject *parent=0);
	
	void initialize();
	void reset();
	void update();
	
private:
	// parameters
    double a, b, c;

	// state
    double T, Tsum;

    // methods
    double temperature(int dayOfYear) const;

    // links
    UniSim::Model *calendar;
	

};


}
#endif