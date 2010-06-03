/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_PULL_VARIABLE_H
#define UNISIM_PULL_VARIABLE_H

#include <QObject>
#include "identifier.h"

namespace UniSim{

class PullVariable : public QObject
{
    Q_OBJECT

public:
    PullVariable(Identifier name, const double *value, QObject *parent);
    double value() const;
    const double* valuePtr() const;
    Identifier id() const;

private:
    Identifier _id;
    const double *_valuePtr;
};


} //namespace

#endif

