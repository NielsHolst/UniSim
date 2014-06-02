/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_FACTORY_PLUG_IN_H
#define UNISIM_FACTORY_PLUG_IN_H

#include <QtPlugin>
#include "exception.h"
#include "identifier.h"
#include "product_base.h"

class QObject;

namespace UniSim{

class FactoryPlugIn
{
public:
    virtual Identifier id() const = 0;
//    virtual ProductList* productList() = 0;
    virtual QList<Identifier> inventory() = 0;
    virtual QObject* create(Identifier className, Identifier objectName, QObject *parent) = 0;
};

} //namespace

Q_DECLARE_INTERFACE(UniSim::FactoryPlugIn, "org.ecolmod.UniSim.FactoryPlugIn")

#endif
