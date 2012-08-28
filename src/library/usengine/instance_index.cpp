/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/data_grid.h>
#include <usbase/exception.h>
#include "instance_index.h"

namespace UniSim{

InstanceIndex::InstanceIndex() {
    table = 0;
}

InstanceIndex::~InstanceIndex() {
    delete table;
}


} //namespace
