/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

#ifndef UNISIM_PRODUCT_BASE_H
#define UNISIM_PRODUCT_BASE_H

#include <QObject>
#include <usbase/identifier.h>

namespace UniSim{

class FactoryPlugIn;

class ProductBase : public QObject
{
public:
    ProductBase(Identifier name, FactoryPlugIn *parent, QString description);
    Identifier id() const;
    FactoryPlugIn* factory() const;
    QString description() const;
    virtual QObject* create(Identifier objectName, QObject *parent=0) const = 0;
private:
    Identifier _id;
    FactoryPlugIn *_factory;
    QString _description;
};

} //namespace

#endif
