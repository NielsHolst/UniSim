/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_PRODUCT_H
#define UNISIM_PRODUCT_H
#include <QObject>
#include <usbase/identifier.h>
#include "product_base.h"

#define PUBLISH(name) \
Product<name> name##Product(#name, productList());

#define PUBLISH2(name, pubname) \
Product<name> name##Product(#pubname, productList());

namespace UniSim{

template <class T>
class Product : public ProductBase
{
public:
    Product(Identifier name, ProductList &list);
    QObject* create(Identifier objectName, QObject *parent) const;
};

template <class T>
Product<T>::Product(Identifier name, ProductList &list)
    :  ProductBase(name, list)
{
}

template <class T>
QObject* Product<T>::create(Identifier objectName, QObject *parent) const
{
    return new T(objectName, parent);
}

} //namespace

#endif
