/* Copyright (C) 2013 by Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

#ifndef INSECTICIDE_FACTORY_H
#define INSECTICIDE_FACTORY_H

#include <QObject>
#include <usbase/factory_plug_in.h>

namespace dynamic_photosynthesis {

class DynamicPhotosynthesis : public QObject, public UniSim::FactoryPlugIn
{
    Q_OBJECT
    Q_INTERFACES(UniSim::FactoryPlugIn)
    Q_PLUGIN_METADATA(IID "org.ecolmod.UniSim")
    public:
    UniSim::Identifier id() const;
    QList<UniSim::Identifier> inventory();
    QObject* create(UniSim::Identifier className, UniSim::Identifier objectName, QObject *parent);
};

} //namespace

#endif
