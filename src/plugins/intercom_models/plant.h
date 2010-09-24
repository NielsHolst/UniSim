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

class Organ;

class Plant : public UniSim::Model
{
	Q_OBJECT
public: 
	Plant(UniSim::Identifier name, QObject *parent=0);
	//standard methods
    void reset();
    void update();
    // special methods
    PhotosyntheticRate calcPhotosynthesis();

private:
    // pull variables
    double heights[5],
        ELAIdiffuse[5], ELAIdirectdirect[5], ELAIdirecttotal[5] ;

	// state
    PhotosyntheticRate photosyntheticRate;
};

} //namespace
#endif
