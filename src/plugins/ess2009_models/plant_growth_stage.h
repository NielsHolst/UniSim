/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef ESS2009_PLANT_GROWTH_STAGE_H
#define ESS2009_PLANT_GROWTH_STAGE_H

#include <QObject>
#include <usbase/model.h>

using UniSim::Model;

namespace ess2009 {

class PlantGrowthStage : public Model
{
    Q_OBJECT
public:
    PlantGrowthStage(UniSim::Identifier name, QObject *parent=0);
	void initialize();
	void reset();
	void update();
private:
    // pull variables
	double _outflowAsDensity, _outflowAsDensityEqs;

    // push variables
    double inflowAsDensity, inflowAsDensityEqs, instantMortality;

	//links
    Model *_density, *_densityEqs;
};

}
#endif
