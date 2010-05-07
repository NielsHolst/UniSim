/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INTERCOM_CANOPY_LAYER
#define INTERCOM_CANOPY_LAYER
#include <QObject>
#include "light_components.h"
#include "photosynthetic_rate.h"

namespace UniSim {
    class Model;
}

namespace intercom{

class Area;
class Plant;

class CanopyLayer : public QObject
{
	Q_OBJECT
public: 
    CanopyLayer(int layer, QObject *parent);
    PhotosyntheticRate calcPhotosynthesis();
    LightComponents calcEffectiveAreaAbove(double height);

private:
    int layer;
    QList<Area*> allAreas, plantAreas;
    UniSim::Model *plantHeight;

    void lookup();
};

} //namespace
#endif
