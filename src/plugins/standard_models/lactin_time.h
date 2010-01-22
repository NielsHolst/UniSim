/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_LACTIN_TIME
#define UNISIM_LACTIN_TIME
#include <QObject>
#include <usbase/model.h>

namespace UniSim{

class LactinTime : public UniSim::Model
{
	Q_OBJECT
public: 
    LactinTime(UniSim::Identifier name, QObject *parent=0);
	//standard methods
	void initialize();
	void reset();
	void update();

private:
	// parameters
    double a, b, c, d;

	// state
    double step, total;

    // models
    UniSim::Model *weather;
};

} //namespace
#endif
