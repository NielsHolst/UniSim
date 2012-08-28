/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef LAKEOXYGEN_LAKEENVIRONMENT
#define LAKEOXYGEN_LAKEENVIRONMENT
#include <QObject>
#include <usbase/model.h>

namespace lakeoxygen {

class LakeEnvironment : public UniSim::Model
{
	Q_OBJECT
public: 
	LakeEnvironment(UniSim::Identifier name, QObject *parent=0);
	// standard methods
    void initialize();
	void update();

private:
	// parameters
    double avgDepth, kd;

    // pull variables
    double light;

    // links
    UniSim::Model *weather;
};

} //namespace
#endif
