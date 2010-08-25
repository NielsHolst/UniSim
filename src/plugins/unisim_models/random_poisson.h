/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_RANDOM_POISSON
#define UNISIM_RANDOM_POISSON
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <QObject>
#include "random_base.h"

namespace UniSim{

class RandomPoisson : public RandomBase
{
	Q_OBJECT
public: 
    RandomPoisson(UniSim::Identifier name, QObject *parent=0);
    ~RandomPoisson();
    // standard methods
    void update();
private:
    typedef boost::mt19937 Generator ;
    typedef boost::uniform_real<double> Distribution;
    typedef boost::variate_generator<Generator&, Distribution> Variate;
    Generator *generator;
    Distribution *distribution;
    Variate *variate;
};

} //namespace
#endif
