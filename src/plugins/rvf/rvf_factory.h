/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef RVF_FACTORY_H
#define RVF_FACTORY_H

#include <usbase/factory_plug_in.h>

namespace rvf {

class RvfFactory : public QObject, public UniSim::FactoryPlugIn
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
