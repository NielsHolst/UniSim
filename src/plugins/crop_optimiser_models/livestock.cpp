/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
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

    //pull parameters, added 12.01.2012
    new PullVariable<int>("Number", &number, this, "Description");
    new PullVariable<double>("AUKey", &AUKey, this, "Description");
    new PullVariable<double>("Nusable", &Nusable, this, "Description");
    new PullVariable<double>("FUuKey", &FUuKey, this, "Description");


    new PushVariable<double>("NanimUsable", &NanimUsable, this, "Description");
    new PushVariable<double>("FUdemand", &FUdemand, this, "Description");

    new PullVariable<double>("NanimUsable", &NanimUsable, this, "Description");
    new PullVariable<double>("FUdemand", &FUdemand, this, "Description");
}

void Livestock::initialize(){

}

void Livestock::reset() {

}

void Livestock::update() {

}

} //namespace


