/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include "livestock.h"

using namespace UniSim;


namespace crop_optimiser {

Livestock::Livestock(UniSim::Identifier name, QObject *parent)
        : Model(name, parent)
{       
    new Parameter<int>("Number", &number, 0, this, "Description");
    new Parameter<double>("AUKey", &AUKey, 0, this, "Description");
    new Parameter<double>("Nusable", &Nusable, 0, this, "Description");
    new Parameter<double>("FUuKey", &FUuKey, 0, this, "Description");
    new Parameter<double>("NanimUsable", &NanimUsable, 0., this, "Description");
    new Parameter<double>("FUdemand", &FUdemand, 0., this, "Description");
}

void Livestock::initialize(){

}

void Livestock::reset() {

}

void Livestock::update() {

}

} //namespace


