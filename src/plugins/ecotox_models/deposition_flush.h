/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef ECOTOX_DEPOSITION_FLUSH
#define ECOTOX_DEPOSITION_FLUSH
#include <boost/random/uniform_01.hpp>
#include <QObject>
#include <usbase/model.h>

namespace UniSim {
    class Random;
    class Generator;
}

namespace ecotox {

class DepositionFlush : public UniSim::Model
{
	Q_OBJECT
public: 
    DepositionFlush(UniSim::Identifier name, QObject *parent=0);
    ~DepositionFlush();
	// standard methods
    void initialize();
	void reset();
	void update();

private:
    // parameters
    int duration;

    // derived
    double scaling;

    // pull variables
    double value, total, percentile;

    // links
    const double *depositionTotal;

    // state
    enum {Before, Inside, After} phase;
    int phaseTime;

    // methods
    inline double f(int x);
    void calcScaling();

    // random number generation
    typedef boost::uniform_01<double>  Distribution;
    typedef boost::variate_generator<UniSim::Random::Generator&, Distribution> Variate;
    Distribution *distribution;
    Variate *variate;
};

double DepositionFlush::f(int x) {
    return x*(duration-x);
}


} //namespace
#endif
