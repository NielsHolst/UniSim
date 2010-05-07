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
#include "light_components.h"
#include "photosynthetic_rate.h"

namespace intercom{

class CanopyLayer;
class Organ;

class Plant : public UniSim::Model
{
	Q_OBJECT
public: 
	Plant(UniSim::Identifier name, QObject *parent=0);
	//standard methods
	void initialize();
	void reset();

    // special methods
    PhotosyntheticRate calcPhotosynthesis();

private:
	// parameters

	// state
    PhotosyntheticRate photosyntheticRate;

    // children
    QList<CanopyLayer*> canopyLayers;

    // links
    UniSim::Models childrenToUpdate;
};

} //namespace
#endif
