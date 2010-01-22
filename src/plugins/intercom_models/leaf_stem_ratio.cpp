/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "leaf_stem_ratio.h"

namespace intercom{

LeafStemRatio::LeafStemRatio(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    setState("ratio", &ratio);
}

void LeafStemRatio::initialize()
{
    setParameter("initial", &initial, 0.73);
    setParameter("slope", &slope, 5.7e-4);
    photoThermalTime = findSibling<Model*>("photoThermalTime");
}

void LeafStemRatio::update()
{
    double pt = photoThermalTime->state("total");
    ratio = initial - slope*pt;
    if (ratio < 0.)
        ratio = 0.;
}

} //namespace

