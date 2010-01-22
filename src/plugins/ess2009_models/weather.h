/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef ESS2009_WEATHER_H
#define ESS2009_WEATHER_H

#include <QObject>
#include <usbase/model.h>

namespace ess2009 {

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
    double _tempOffset, _tempB, _tempC, _tempD;

	// state
    double _T,_Tsum;

    // methods
    double temperature(int dayOfYear) const;

    // links
    UniSim::Model *_calendar;
	

};


}
#endif
