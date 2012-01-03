/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef ANIMALSTOCKING_GRASSPRODUCTIVITY_REMOTE_ESTIMATE
#define ANIMALSTOCKING_GRASSPRODUCTIVITY_REMOTE_ESTIMATE
#include <QObject>
#include "grass_productivity.h"

namespace AnimalStocking {

class GrassProductivityRemoteEstimate : public GrassProductivity
{
	Q_OBJECT
public: 
    GrassProductivityRemoteEstimate(UniSim::Identifier name, QObject *parent=0);
    // special methods
    double calculateDryMatter();
private:
    // parameters
    double slope, intercept;
};

} //namespace
#endif
