/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/data_grid.h>
#include "instance_index_numbered.h"

namespace UniSim{

InstanceIndexNumbered::InstanceIndexNumbered(QString objectName_, int numInstances_)
    : InstanceIndex(), objectName(objectName_), numInstances(numInstances_), nextInstance(0)
{
}

void InstanceIndexNumbered::reset(QString, Model *) {
    nextInstance = 0;
}

bool InstanceIndexNumbered::hasNext() {
    return nextInstance < numInstances;
}

InstanceIndex::Record InstanceIndexNumbered::next() {
    Q_ASSERT(hasNext());
    InstanceIndex::Record rec;
    rec.modelName = QString("%1(%2)").arg(objectName).arg(++nextInstance);
    return rec;
}


} //namespace

