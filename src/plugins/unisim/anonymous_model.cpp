/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <QString>
#include "anonymous_model.h"

namespace UniSim{
	
AnonymousModel::AnonymousModel(Identifier name, QObject *parent)
	: Model(name, parent)
{
}

} //namespace

