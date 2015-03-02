/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
//#include <usbase/exception.h>
#include "leafoverallrateconstant.h"
#include "publish.h"


using namespace UniSim;

namespace insecticide {

PUBLISH (leafoverallrateconstant)
	
leafoverallrateconstant::leafoverallrateconstant(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, klu, 0.);       //h-1
    Input (double, klv, 0.);       //h-1
    Input (double, kld, 0.);       //h-1

    Output (double, kov);          //overall rate constant of leaf removal processes (h-1)

}

void leafoverallrateconstant::reset() {

    kov = 0.;
}

void leafoverallrateconstant::update() {

    kov = klv + klu + kld;    

}

} //namespace

