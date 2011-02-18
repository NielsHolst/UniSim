/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "output.h"

namespace UniSim{
	
Output::Output(QString name, QObject *parent)
    : Component(name, parent)
{
}

void Output::initialize() {
    new Parameter<double>("title", &title, "untitled");
}

} //namespace

