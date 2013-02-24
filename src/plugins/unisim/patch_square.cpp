/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "patch_square.h"

namespace UniSim{

PatchSquare::PatchSquare(UniSim::Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Parameter<int>("x", &x, 0., this,
        "Lateral position");
    new Parameter<int>("y", &y, 0., this,
        "Vertical position");
    new Parameter<int>("size", &size, 10., this,
        "Edge length of square");
}

} //namespace

