/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_VARIABLE_BASE_H
#define UNISIM_VARIABLE_BASE_H

#include "identifier.h"

namespace UniSim{

class VariableBase : public QObject
{
    // no Q_OBJECT
public:
    VariableBase(Identifier id, QObject *parent, QString desc);
    virtual QVariant toVariant() const = 0;
    virtual QString toString() const = 0;
    virtual QString typeId() const = 0;
    Identifier id() const;
    QString description() const;

private:
    // methods
    void assertUniqueness();
    // data
    Identifier _id;
    QString _description;
};

} //namespace

#endif

