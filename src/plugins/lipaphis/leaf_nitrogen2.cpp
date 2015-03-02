/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "leaf_nitrogen2.h"
#include "publish.h"
#include <cmath>

using namespace UniSim;

namespace lipaphis {

PUBLISH(LeafNitrogen2)

LeafNitrogen2::LeafNitrogen2(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, fertilisation, 225.);
    Input(double, CumuDegree, 0.);
    Output(double, percentage);
}

void LeafNitrogen2::reset() {
    percentage = 1.;
}

void LeafNitrogen2::update() {
    double x = fertilisation,
           a = 0.0124*x + 2.3311,
           b = 0.4064*x + 512.82,
           c = 0.74386;
    percentage = a* exp(-0.5* pow(log(CumuDegree/b)/c,2));
}

} //namespace

