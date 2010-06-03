/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/pull_variable.h>
#include "leaf_stem_ratio.h"

using namespace UniSim;

namespace intercom{

LeafStemRatio::LeafStemRatio(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    new PullVariable("ratio", &ratio, this);
}

void LeafStemRatio::initialize()
{
    setParameter("initial", &initial, 0.73);
    setParameter("slope", &slope, 5.7e-4);
    photoThermalTime = seekOneSibling<Model*>("photoThermalTime");
}

void LeafStemRatio::update()
{
    double pt = photoThermalTime->pullVariable("total");
    ratio = initial - slope*pt;
    if (ratio < 0.)
        ratio = 0.;
}

} //namespace

