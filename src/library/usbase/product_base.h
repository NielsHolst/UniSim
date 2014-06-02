/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

#ifndef UNISIM_PRODUCT_BASE_H
#define UNISIM_PRODUCT_BASE_H

#include <QMap>
#include <usbase/identifier.h>

class QObject;

namespace UniSim{

class ProductBase;
typedef QMap<Identifier, ProductBase*> ProductList;

class ProductBase
{
public:
    ProductBase(Identifier name, ProductList &list);
    virtual QObject* create(Identifier objectName, QObject *parent) const = 0;
    Identifier id() const;
private:
    Identifier _id;
};

} //namespace

#endif
