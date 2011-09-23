/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef SUPPLYDEMAND_EXPOSURE
#define SUPPLYDEMAND_EXPOSURE
#include <QObject>
#include <usbase/model.h>

namespace SupplyDemand {

class Exposure : public UniSim::Model
{
	Q_OBJECT
public: 
    Exposure(UniSim::Identifier name, QObject *parent=0);
	// standard methods
    void initialize();
    void reset();
    void update();

private:
	// parameters

    // pull variables
    double current, total;

    // links
    const double *larvae, *pollen;
};

} //namespace
#endif
