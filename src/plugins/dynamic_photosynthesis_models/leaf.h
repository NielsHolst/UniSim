/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef DYNAMIC_PHOTOSYNTHESIS_LEAF
#define DYNAMIC_PHOTOSYNTHESIS_LEAF
#include <QObject>
#include <usbase/model.h>

namespace dynamic_photosynthesis {

class Leaf : public UniSim::Model
{
	Q_OBJECT
public: 
        Leaf(UniSim::Identifier name, QObject *parent=0);
	// standard methods
        void initialize();
        void reset();
	void update();

private:
	// parameters
        double Pini, Rd, a, b, PPFDmax, eqrate, poseqrate, widtheqrate, eqmax, phiA, alpha, AmaxSp;

	// state
        double P,Peq,Amax, Aind;
        UniSim::Model* weather;

        // methods
        double calcAmax(double PPFD);
        void calceqrate(double PPFD);
        void calcPeq(double PPFD);
};

} //namespace
#endif
