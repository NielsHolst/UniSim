/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

#include "model.h"
#include "pull_variable.h"
#include "push_variable.h"

using namespace std;

namespace UniSim{
	
Model::Model(Identifier name, QObject *parent)
    : Component(name, parent)
{
}

/*
void Model::pushVariable(Identifier name, double value) {
    seekOneChild<PushVariable*>(name.key())->setValue(value);
}
*/
} //namespace

