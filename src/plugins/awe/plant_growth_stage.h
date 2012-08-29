/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef AWE_PLANT_GROWTH_STAGE_H
#define AWE_PLANT_GROWTH_STAGE_H

#include <QObject>
#include <usbase/model.h>

using UniSim::Model;

namespace awe {

class PlantGrowthStage : public Model
{
    //Q_OBJECT
public:
    PlantGrowthStage(UniSim::Identifier name, QObject *parent=0);
	void initialize();
	void reset();
	void update();
private:
    // pull variables
    double outflowAsDensity, outflowAsDensityEqs;

    // push variables
    double inflowAsDensity, inflowAsDensityEqs, instantMortality;

	//links
    Model *density, *densityEqs;
};

}
#endif
