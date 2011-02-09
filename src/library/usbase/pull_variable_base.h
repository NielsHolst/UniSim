/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_PULL_VARIABLE_BASE_H
#define UNISIM_PULL_VARIABLE_BASE_H

#include <QObject>
#include "identifier.h"

namespace UniSim{

class PullVariableBase : public QObject
{
    // no Q_OBJECT
public:
    PullVariableBase(Identifier id, QObject *parent, QString desc);
    virtual QVariant toVariant() const = 0;
    virtual QString typeId() const = 0;
    Identifier id() const;
    QString description() const;

private:
    Identifier _id;
    QString _description;
};

} //namespace

#endif

