/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_IDENTIFIER_H
#define UNISIM_IDENTIFIER_H
#include <QString>

namespace UniSim {

class Identifier {
public:
    Identifier();
    Identifier(QString label);
    Identifier(const char *label);
    QString key() const;
    QString label() const;
    QString withoutNamespace() const;
    bool equals(QString label) const;
    bool equals(const char *label) const;
private:
    void makeKey();
    QString _label, _key;
};

bool operator<(const Identifier &id1, const Identifier &id2);
bool operator==(const Identifier &id1, const Identifier &id2);

typedef QList<Identifier> Identifiers;

} //namespace

#endif
