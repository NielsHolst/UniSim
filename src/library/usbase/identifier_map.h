/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_IDENTIFIER_MAP_H
#define UNISIM_IDENTIFIER_MAP_H
#include <QMap>
#include <QString>
#include "identifier.h"

namespace UniSim {

class IdentifierMap : public QMap<Identifier, QString> {
public:
    IdentifierMap();
    void parse(QString s);
private:
};

} //namespace

#endif
