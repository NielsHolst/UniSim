/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef SUPPLYDEMAND_SPLIT_ON_SHORTENING_DAYS
#define SUPPLYDEMAND_SPLIT_ON_SHORTENING_DAYS
#include <QObject>
#include <usbase/model.h>

namespace SupplyDemand {

class SplitOnShorteningDays : public UniSim::Model
{
	//Q_OBJECT
public: 
    SplitOnShorteningDays(UniSim::Identifier name, QObject *parent=0);
	// standard methods
    void initialize();
    void reset();
	void update();

private:
	// parameters
    double critDayLength;

    // pull variables
    double first, second;

    // links
    const double *dayLength;
    const int *dayOfYear;
};

} //namespace
#endif
