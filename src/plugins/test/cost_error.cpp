/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/name.h>
#include "cost_error.h"
#include "publish.h"

using namespace UniSim;

namespace test{

PUBLISH(CostError)

CostError::CostError(Identifier name, QObject *parent)
	: Model(name,parent) 
{ 
    addParameterRef<int>(Name(energy), "operation[na]");
}

} //namespace

