/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef SUPPLYDEMAND_LIFE_STAGE
#define SUPPLYDEMAND_LIFE_STAGE
#include <QObject>
#include <usbase/model.h>

namespace SupplyDemand {

class LifeStage : public UniSim::Model
{
	Q_OBJECT
public: 
    LifeStage(UniSim::Identifier name, QObject *parent=0);
	// standard methods

private:
	// parameters

    // pull variables

    // links
};

} //namespace
#endif
