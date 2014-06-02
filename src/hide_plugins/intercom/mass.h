/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INTERCOM_MASS
#define INTERCOM_MASS
#include <QObject>
#include <usbase/model.h>

namespace intercom{

class Mass : public UniSim::Model
{
	//Q_OBJECT
public: 
    enum KindsOfLight {Diffuse, DirectDirect, DirectTotal};
    enum Rates {Absorption, Assimilation};

    Mass(UniSim::Identifier name, QObject *parent=0);

	//standard methods
    void initialize();
	void reset();
    void update();

    // special methods
    void updateCurrentPartition(double total);

private:
    // pull variables
    double value, currentPartition, allocationRate;
    // push variables
    double allocation;
    // links
    UniSim::Model *mass;
};

} //namespace
#endif
