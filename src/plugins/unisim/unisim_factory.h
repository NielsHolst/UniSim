/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_FACTORY_H
#define UNISIM_FACTORY_H

#include <QObject>
#include <usbase/factory_plug_in.h>

namespace UniSim{

class UniSimFactory : public QObject, public FactoryPlugIn
{
	Q_OBJECT
    Q_INTERFACES(UniSim::FactoryPlugIn)
public:
    void defineProducts();
    UniSim::Identifier id() const;
    QString description() const;
    QStringList authors() const;
    QObject* asQObject();
};

} //namespace

#endif


