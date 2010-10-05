/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INTERCOM_LAYER
#define INTERCOM_LAYER
#include <QMap>
#include <QObject>
#include <usbase/named_object.h>
#include "light_components.h"
#include "photosynthetic_rate.h"

namespace UniSim {
    class Model;
}

namespace intercom{

class Area;
class Plant;

class Layer : public UniSim::NamedObject
{
	Q_OBJECT
public: 
    Layer(UniSim::Identifier name, QObject *parent, int layer);
    void initialize();
    PhotosyntheticRate calcPhotosynthesis();
    LightComponents calcEffectiveAreaAbove(double height);
    LightComponents ELAI();
    double height() const;
    QMap<Area*, LightComponents> absorptionByArea();

private:
    // state
    int layer;

    // links
    QList<Area*> allAreas, plantAreas;
    UniSim::Model *plantHeight, *calendar;
};

} //namespace
#endif
