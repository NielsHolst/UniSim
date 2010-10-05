/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INTERCOM_PLANT_LAYERS
#define INTERCOM_PLANT_LAYERS
#include <QObject>
#include <usbase/named_object.h>

namespace intercom{

class Layer;
class Plant;

class PlantLayers : public UniSim::NamedObject
{
	Q_OBJECT
public: 
    PlantLayers(UniSim::Identifier name, Plant *plant, QObject *parent);
    void initialize();
    Plant* plant();
private:
    // children
    QList<Layer*> layers;
    // links
    Plant *_plant;
};

} //namespace
#endif
