/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "leaf_nitrogen.h"
#include "publish.h"

using namespace UniSim;

namespace lipaphis {

PUBLISH(LeafNitrogen)

LeafNitrogen::LeafNitrogen(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, fertilisation, 225.);
    Input(int, time, 0.);
    Output(double, percentage);
}

void LeafNitrogen::reset() {
    percentage = 1.;
}

void LeafNitrogen::update() {
    double x = fertilisation,
           a = 0.0122*x + 2.3135,
           b = 0.0347*x + 55.182,
           c = 32.17528;

    percentage = a*exp(-exp(-(time-b)/c)-(time-b)/c+1);
}

} //namespace

