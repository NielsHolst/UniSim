/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_VARIABLE_BASE_H
#define UNISIM_VARIABLE_BASE_H

#include <QObject>
#include "identifier.h"
#include "reference.h"

namespace UniSim{

class NamedObject;

class VariableBase : public QObject
{
public:
    VariableBase(Identifier id, NamedObject *parent);
    virtual void initializeValue() = 0;
    virtual void resetValue() = 0;
    virtual QVariant toVariant() const = 0;
    virtual QString toString() const = 0;
    virtual QString typeId() const = 0;
    virtual void resolveReference() = 0;
    virtual void followReference() = 0;
    Identifier id() const
        { return _id; }
    const NamedObject* parent() const
        { return _parent; }
    void from(Reference reference)
        { _reference = reference; }
    bool isReference()
        { return !_reference.isEmpty(); }
private:
    void assertUniqueness();
protected:
    Identifier _id;
    NamedObject *_parent;
    Reference _reference;
};

} //namespace

#endif

