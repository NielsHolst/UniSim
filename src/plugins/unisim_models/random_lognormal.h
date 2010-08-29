/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_RANDOM_LOGNORMAL
#define UNISIM_RANDOM_LOGNORMAL
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/lognormal_distribution.hpp>
#include <boost/random/variate_generator.hpp>
#include <QObject>
#include "random_base.h"

namespace UniSim{

class RandomLognormal : public RandomBase
{
	Q_OBJECT
public: 
    RandomLognormal(UniSim::Identifier name, QObject *parent=0);
    ~RandomLognormal();
    // standard methods
    void initialize();
private:
    // parameters
    double mean, sd;
    // methods
    double drawValue();
    // random number generation
    typedef boost::mt19937 Generator ;
    typedef boost::lognormal_distribution<double> Distribution;
    typedef boost::variate_generator<Generator&, Distribution> Variate;
    Generator *generator;
    Distribution *distribution;
    Variate *variate;
};

} //namespace
#endif
