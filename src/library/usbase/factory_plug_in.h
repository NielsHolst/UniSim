/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

#ifndef UNISIM_FACTORY_PLUG_IN_H
#define UNISIM_FACTORY_PLUG_IN_H

#include <QMap>
#include <QtPlugin>
#include <usbase/exception.h>
#include <usbase/identifier.h>
#include <usbase/utilities.h>
#include "product.h"

class QObject;

namespace UniSim{

class FactoryPlugIn
{
public:
    typedef QMap<Identifier, const ProductBase*> Products;
    virtual ~FactoryPlugIn() { }
    virtual void defineProducts() = 0;
    virtual Identifier id() const = 0;
    virtual QString description() const = 0;
    virtual QStringList authors() const = 0;
    virtual QObject* asQObject() = 0;
private:
    Products _products;
    friend const Products& products(FactoryPlugIn *factory);

    template <class T>
    friend void addProduct(Identifier id, FactoryPlugIn *parent, QString desc);

    friend QObject* create(FactoryPlugIn *factory, Identifier className, Identifier objectName, QObject *parent);
};

inline const FactoryPlugIn::Products& products(FactoryPlugIn *factory) {
    return factory->_products;
}

template <class T>
inline void addProduct(Identifier id, FactoryPlugIn *parent, QString desc) {
    ProductBase *product = new Product<T>(id, parent, desc);
    if (parent->_products.contains(id)) {
        QString msg = "Factory '%1' already contains a product called '%2'";
        throw Exception(msg.arg(parent->id().label().arg(id.label())));
    }
    parent->_products[id] = product;
}

//inline void addProduct(const ProductBase *product) {
//    FactoryPlugIn *factory = product->factory();
//    Identifier id = product->id();
//    Q_ASSERT(!factory->products.contains(id));
//    factory->products[id] = product;
//}

inline QObject* create(FactoryPlugIn *factory, Identifier className, Identifier objectName, QObject *parent)
{
    setSimulationObjectFromDescendent(parent);
    FactoryPlugIn::Products::const_iterator p = factory->_products.find(className);
    return (p==factory->_products.end()) ? 0 : p.value()->create(objectName, parent);
}

} //namespace

Q_DECLARE_INTERFACE(UniSim::FactoryPlugIn, "org.ecolmod.UniSim.FactoryPlugIn")

#endif
