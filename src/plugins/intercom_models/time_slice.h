/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INTERCOM_TIME_SLICE
#define INTERCOM_TIME_SLICE
#include <QObject>
#include <usbase/named_object.h>
#include "photosynthetic_rate.h"

namespace UniSim {
    class Model;
}

namespace intercom{

class PlantLayers;
class Plant;

class TimeSlice : public UniSim::NamedObject
{
	Q_OBJECT
public: 
    TimeSlice(UniSim::Identifier name, QObject *parent, int slice);
    void initialize();

    void tick();
    double hour();
    double dayLength();
    int index() const;

    PhotosyntheticRate calcPhotosynthesis();
private:
    // state
    int slice;

    // children
    QList<PlantLayers*> plantLayers;

    // links
    QList<Plant*> plants;
    UniSim::Model *calendar;
};

} //namespace
#endif
