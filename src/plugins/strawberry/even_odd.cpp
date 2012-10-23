/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "even_odd.h"

using namespace UniSim;

namespace strawberry {
	
EvenOdd::EvenOdd(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<int>("initialValue", &initialValue, 100, this, "desc");
    new Variable<int>("value", &value, this, "desc");
}

void EvenOdd::reset() {
    value = initialValue;
}

void EvenOdd::update() {
    if (value%2 == 0)
        value /= 2;
    else
        value = 3*value + 1;
}


} //namespace

