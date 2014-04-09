/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include <usbase/variable.h>
#include "holometabola.h"
#include "life_stage.h"

using namespace UniSim;


namespace SupplyDemand {

LifeStage::LifeStage(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
}


} //namespace

