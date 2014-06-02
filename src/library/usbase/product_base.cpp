/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

#include "product_base.h"

namespace UniSim{

ProductBase::ProductBase(Identifier id, ProductList &list)
    : _id(id)
{
    ProductList test = list;
    list[id] = this;
}

Identifier ProductBase::id() const {
    return _id;
}

} //namespace
