/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

#ifndef UNISIM_PRODUCT_H
#define UNISIM_PRODUCT_H

#include "product_base.h"

namespace UniSim{

class FactoryPlugIn;

template <class T>
class Product : public virtual ProductBase

{
public:
    Product(Identifier id, FactoryPlugIn *parent, QString desc);
    QObject* create(Identifier objectName, QObject *parent) const;
};

template <class T>
Product<T>::Product(Identifier id, FactoryPlugIn *parent, QString desc)
    :  ProductBase(id, parent, desc)
{
}

template <class T>
QObject* Product<T>::create(Identifier objectName, QObject *parent) const
{
    return new T(objectName, parent);
}

} //namespace

#endif
