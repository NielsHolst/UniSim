/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QRegExp>
#include "dataset.h"
#include "parameter.h"

namespace UniSim{
	
Dataset::Dataset(Identifier name, QObject *parent = 0)

    : Component(name, parent)
{
    new Parameter<QString>("fileName", &fileName, QString("observations.txt"), this,
    "Name of file with data to plot along with simulated curves");
}

void Dataset::reset() {
    _history.clear();
}

void Dataset::update() {
    Q_ASSERT(pullVarPtr);
    double toAppend = pullVarPtr->toVariant().value<double>();
    _history.append(toAppend);
}

} //namespace

