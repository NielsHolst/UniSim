/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef BEEHIVE_FECUNDITY
#define BEEHIVE_FECUNDITY
#include <QObject>
#include <usbase/model.h>

namespace beehive{

class Fecundity : public UniSim::Model
{
	Q_OBJECT
public: 
    Fecundity(UniSim::Identifier name, QObject *parent=0);
	// standard methods
	void initialize();
	void reset();
	void update();

private:
    // methods
    double fecundity(double age) const;

    // parameters
    double scale, root1, root2, sexRatio;

    // pull variables
    double eggsLaid;

    // links
    UniSim::Model *adult;
};

} //namespace
#endif
