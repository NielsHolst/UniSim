/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include "leaf_stem_ratio.h"

using namespace UniSim;

namespace intercom{

LeafStemRatio::LeafStemRatio(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("initial", &initial, 0.73, this, "description");
    new Parameter<double>("slope", &slope, 5.7e-4, this, "description");
    new Variable<double>("ratio", &ratio, this, "description");
}

void LeafStemRatio::initialize()
{
    photoThermalTime = seekOneNearest<Model*>("time");
}

void LeafStemRatio::update()
{
    double pt = photoThermalTime->pullValue<double>("total");
    ratio = initial - slope*pt;
    if (ratio < 0.)
        ratio = 0.;
}

} //namespace

