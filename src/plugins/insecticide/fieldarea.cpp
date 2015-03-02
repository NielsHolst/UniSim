/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "fieldarea.h"
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (fieldarea)
	
fieldarea::fieldarea(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, W, 110.);     //field width (m)
    Input (double, L, 90.);      //field length (m)

    Output (double, area);

}

void fieldarea::reset() {

    area = 0.;

}

void fieldarea::update() {

    area = W * L;

}

} //namespace

