/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef AMBROSIA_PLANT
#define AMBROSIA_PLANT
#include <QObject>
#include <usbase/model.h>

namespace ambrosia{

class Plant : public UniSim::Model
{
	Q_OBJECT
public: 
    Plant(UniSim::Identifier name, QObject *parent=0);
	// standard methods
	void initialize();
	void reset();
	void update();

private:
	// parameters
    enum {NumStages = 5};
    double bioDays[NumStages];
    int beginDay, beginMonth;

	// state
    double stage, total;

    // model
    UniSim::Model *calendar, *timeABDE, *timeC;
};

} //namespace
#endif
