/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include "output.h"
#include "output_destination.h"

namespace UniSim{

OutputDestination::OutputDestination(QString name, Output *parent)
    : Component(name, parent), _output(parent)
{
    Q_ASSERT(parent);
}
	
void OutputDestination::cleanup() {
	showData();

}
void OutputDestination::setXYStateVariablesList(const QList<XYStateVariables*> *xyList) {
    _xyList = xyList;
}

}
