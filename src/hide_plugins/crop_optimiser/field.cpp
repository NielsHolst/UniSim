/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include "field.h"

using namespace UniSim;


namespace crop_optimiser {

Field::Field(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
        new Parameter<double>("Area", &area, 1., this, "Description");
        new Parameter<QString>("CropIdentifier", &cropIdentifier, "WWheat", this, "Description");
    //crops = new CropParameters("cropParameters", "crop parameters_0711", this, "Description...");
        //new Variable<double>("N", &density, this, "Description");
}

void Field::reset() {

}

void Field::update() {

}

} //namespace

