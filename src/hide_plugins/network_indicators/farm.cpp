/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "farm.h"

using namespace UniSim;

namespace network_indicators {
	
Farm::Farm(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("deposition", &deposition, 0., this, "desc");
    new Parameter<double>("fixation", &fixation, 0., this, "desc");
    new Parameter<double>("importCrops", &importCrops, 0., this, "desc");
    new Parameter<double>("importManure", &importManure, 0., this, "desc");
    new Parameter<double>("nonSymbFixation", &nonSymbFixation, 0., this, "desc");
    new Parameter<double>("uptakeCrops", &uptakeCrops, 0., this, "desc");
    new Parameter<double>("toAnimals", &toAnimals, 0., this, "desc");
    new Parameter<double>("toManure", &toManure, 0., this, "desc");
    new Parameter<double>("greenManure", &greenManure, 0., this, "desc");
    new Parameter<double>("totalManure", &totalManure, 0., this, "desc");
}

} //namespace

