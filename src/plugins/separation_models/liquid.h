/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef SEPARATION_LIQUID
#define SEPARATION_LIQUID
#include <QObject>
#include <usbase/model.h>

namespace separation {

class Liquid : public UniSim::Model
{
	Q_OBJECT
public: 
	Liquid(UniSim::Identifier name, QObject *parent=0);
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
