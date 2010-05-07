/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INTERCOM_ORGAN
#define INTERCOM_ORGAN
#include <QObject>
#include <usbase/model.h>
#include "light_components.h"

namespace intercom{

class Area;

class Organ : public UniSim::Model
{
	Q_OBJECT
public: 
    Organ(UniSim::Identifier name, QObject *parent=0);
private:
};

} //namespace
#endif
