/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INTERCOM_HEIGHT
#define INTERCOM_HEIGHT
#include <QObject>
#include <usbase/model.h>

namespace intercom{

class Height : public UniSim::Model
{
	Q_OBJECT
public: 
    Height(UniSim::Identifier name, QObject *parent=0);
	//standard methods
	void initialize();
	void update();

private:
	// parameters
    double a, b, c, m;

	// state
    double height;

    // models
    UniSim::Model *photoThermalTime;

};

} //namespace
#endif
