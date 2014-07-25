/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "layer.h"
#include "publish.h"

using namespace UniSim;

namespace vg {

PUBLISH(Layer)

Layer::Layer(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, xGauss, 1);
    Input(double, wGauss, 1);
}



} //namespace

