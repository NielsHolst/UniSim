/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/data_grid.h>
#include "instance_index_one.h"

namespace UniSim{

InstanceIndexOne::InstanceIndexOne(QString objectName_)
    : InstanceIndex(), objectName(objectName_), _hasNext(true)
{
}

void InstanceIndexOne::reset(QString, Model *) {
    _hasNext = true;
}

bool InstanceIndexOne::hasNext() {
    return _hasNext;
}

InstanceIndex::Record InstanceIndexOne::next() {
    Q_ASSERT(_hasNext);
    _hasNext = false;
    InstanceIndex::Record rec;
    rec.modelName = objectName;
    return rec;
}


} //namespace

