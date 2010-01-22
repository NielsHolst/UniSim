/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INTERCOM_PLANT
#define INTERCOM_PLANT
#include <QList>
#include <QObject>
#include <usbase/model.h>
#include "absorption_exponents.h"

namespace intercom{

class Organ;

class Plant : public UniSim::Model
{
	Q_OBJECT
public: 
	Plant(UniSim::Identifier name, QObject *parent=0);
	//standard methods
	void initialize();
	void reset();
	void update();

    // special methods
    AbsorptionExponents absorptionExponents(int layer);
    void prepareUpdate();
    void accumulateLightUseByLayer(int layer);
    void accumulateLightUseByTime(int time);

private:
	// parameters

	// state
    struct AccumLightUse {
        double organ, layer, time;
        void reset() {
            organ = layer = time = 0.;
        }
    };
    AccumLightUse absorption, assimilation;

    // links
    UniSim::Models childrenToUpdate;
    UniSim::Model *height;
    QList<Organ*> organs;
    double plantHeight;
};

} //namespace
#endif
