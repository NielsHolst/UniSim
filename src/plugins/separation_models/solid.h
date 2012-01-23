/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef SEPARATION_SOLID
#define SEPARATION_SOLID
#include <QObject>
#include <usbase/model.h>

namespace separation {

class Solid : public UniSim::Model
{
	Q_OBJECT
public: 
	Solid(UniSim::Identifier name, QObject *parent=0);
	// standard methods
        void initialize();
        void reset();
	void update();

private:
	// parameters
        double TAN, pH, rA,TANmol;
        QString cover, coverlist;

        int area;

        // pull variables
        double Kn, Kh, NH3aq, NH3g, NH3emis, rC, Totalemis;
        UniSim::Model *weather;
        // push variables
        double TANin;

};

} //namespace
#endif
