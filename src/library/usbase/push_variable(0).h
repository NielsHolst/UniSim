/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_PUSH_VARIABLE_H
#define UNISIM_PUSH_VARIABLE_H

#include <QObject>
#include "identifier.h"

namespace UniSim{

class PushVariable : public QObject
{
    Q_OBJECT

public:
    PushVariable(Identifier name, double *valuePtr, QObject *parent, QString desc);
    double value() const;
    double* valuePtr();
    void setValue(double value);
    Identifier id() const;
    QString description() const;

private:
    Identifier _id;
    double *_valuePtr;
    QString desc;
};


} //namespace

#endif

