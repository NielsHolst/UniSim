/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INTERCOM_AREA_DENSITY_TOPHEAVY
#define INTERCOM_AREA_DENSITY_TOPHEAVY
#include <QList>
#include <QMap>
#include <QObject>
#include <usbase/model.h>
#include "area_density.h"

namespace intercom{

class AreaDensityTopheavy : public AreaDensity
{
	Q_OBJECT
public: 
    AreaDensityTopheavy(UniSim::Identifier name, QObject *parent=0);
    // special methods
    double at(double height) const;
    double above(double height) const;
};

} //namespace
#endif
