/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef AMBROSIA_TIME
#define AMBROSIA_TIME
#include <QObject>
#include <usbase/model.h>

namespace ambrosia{

class Time : public UniSim::Model
{
	//Q_OBJECT
public: 
	Time(UniSim::Identifier name, QObject *parent=0);
	// standard methods
	void initialize();
	void reset();
	void update();

private:
	// parameters
    double L0, alfa, T0, Tmax;

	// state
    double dayLengthIndex, temperatureIndex, step, total;

    // model
    UniSim::Model *calendar, *weather;
};

} //namespace
#endif
