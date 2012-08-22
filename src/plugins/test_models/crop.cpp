/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/file_locations.h>
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include "crop.h"

using namespace UniSim;

namespace test{

Crop::Crop(Identifier name, QObject *parent)
	: Model(name,parent) 
{ 
    new Parameter<int>("numFields", &numFields, 1, this, "description");
    new Parameter<int>("Area", &initArea, 100, this, "description");
    new Parameter<double>("nitrogenNorm", &nitrogenNorm, 0., this, "description");
    new Variable<int>("CurrentArea", &currentArea, this, "description");
}

void Crop::reset() {
    currentArea = initArea;
}

void Crop::update() {
    ++currentArea;
}

} //namespace

