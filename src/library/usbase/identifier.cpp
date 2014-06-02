/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QHash>
#include "identifier.h"

/*! \class UniSim::Identifier
    \brief The %Identifier class is used for the names of all classes, parameters, state and input variables

    An %Identifier constructs a standardized key value from the label provided in the constructor. To construct
    the key, the label is turned to lower case, leading and trailing whitespace is removed, and within-string
    whitespace is replaced by an underscore.

    E.g. the label " Cats   and dogs " will result in the key "cats_and_dogs".

    The original %label can be accessed through label() and should be used for for nice-looking labels.
    The key() string is useful e.g. as a column heading in file output, as it contains no blanks.

    Comparison operators '<' and '==' are provided for two Identifiers, and equals() compares the identifier
    to a string label.
*/

namespace UniSim {

Identifier::Identifier()
{
}

Identifier::Identifier(QString label)
    : _label(label)
{
    makeKey();
}

Identifier::Identifier(const char *label)
    : _label(label)
{
    makeKey();
}

void Identifier::makeKey() {
    _key = _label
        .toLower()
        .simplified()
        .replace(" ", "_")
        .replace("(","_")
        .replace(")","_")
        .replace("-","_");
}

QString Identifier::key() const {
    return _key;
}

QString Identifier::label() const {
    return _label;
}

//! Returns key without namespace
QString Identifier::withoutNamespace() const {
    int i = _key.lastIndexOf("::");
    int n = _key.size() - i - 2;
    return (i == -1)  ? _key : _key.right(n);
}

bool Identifier::equals(QString label) const {
    return _key == Identifier(label).key();
}

bool Identifier::equals(const char *label) const {
    return equals(QString(label));
}

Identifier& Identifier::operator=(const Identifier &id) {
    _label = id._label;
    _key = id._key;
    return *this;
}

bool operator<(const Identifier &id1, const Identifier &id2) {
    return id1.key() < id2.key();
}

bool operator==(const Identifier &id1, const Identifier &id2) {
    return id1.key() == id2.key();
}

bool operator!=(const Identifier &id1, const Identifier &id2) {
    return id1.key() != id2.key();
}

uint qHash(Identifier id) {
    return qHash(id.key());
}


}


