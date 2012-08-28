/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef SEED_EMERGENCE_MODEL_MAKER_H
#define SEED_EMERGENCE_MODEL_MAKER_H

#include <usbase/factory_plug_in.h>

namespace seed_emergence {

class SeedEmergenceFactory : public QObject, public UniSim::FactoryPlugIn
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

