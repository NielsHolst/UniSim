/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INTERCOM_LEAF_STEM_RATIO
#define INTERCOM_LEAF_STEM_RATIO
#include <QObject>
#include <usbase/model.h>

namespace intercom{

class LeafStemRatio : public UniSim::Model
{
	//Q_OBJECT
public: 
    LeafStemRatio(UniSim::Identifier name, QObject *parent=0);
	//standard methods
	void initialize();
	void update();

private:
	// parameters
    double initial, slope;

	// state
    double ratio;

    // models
    UniSim::Model *photoThermalTime;

};

} //namespace
#endif
