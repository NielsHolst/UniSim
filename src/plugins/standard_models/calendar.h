/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_CALENDAR
#define UNISIM_CALENDAR
#include <QObject>
#include <usbase/model.h>

namespace UniSim{

class Calendar : public UniSim::Model
{
	Q_OBJECT
public: 
	Calendar(UniSim::Identifier name, QObject *parent=0);
	//standard methods
	void initialize();
	void reset();
	void update();

    // special methods
    double sinb(double hour) const;

private:
	// parameters
    double latitude, startingDayInYear;

	// state
    double dayInYear, daysTotal, year, dayLength, sinLD, cosLD;

};

} //namespace
#endif
