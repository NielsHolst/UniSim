/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/file_locations.h>
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include "farm.h"

using namespace UniSim;

namespace test{

Farm::Farm(Identifier name, QObject *parent)
	: Model(name,parent) 
{ 
    new Parameter<int>("farmRealId", &realId, 12345, this, "description");
    new Parameter<QString>("farmType", &farmType, QString("Plant"), this, "description");
    new Parameter<QString>("soilType", &soilType, QString("Clay"), this, "description");
    new Parameter<QString>("economy", &economicType, QString("Private"), this, "description");
    new Parameter<int>("leakageRate", &leakageRate, -1, this, "description");
}


} //namespace

