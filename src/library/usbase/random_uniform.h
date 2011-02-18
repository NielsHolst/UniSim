/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UniSim_RANDOM_UNIFORM_H
#define UniSim_RANDOM_UNIFORM_H

#include <QObject>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <usbase/exception.h>
#include <usbase/object_pool.h>

namespace UniSim{
	
class RandomUniform : public QObject
{
    Q_OBJECT

public:
    RandomUniform();
    static QString id();
    void setSeed(unsigned seed);
    double next();
private:
    // Singleton
    static RandomUniform* _randomUniform;
    friend RandomUniform* randomUniform();
    // boost
    typedef boost::mt19937 Generator;
    Generator generator;
    boost::uniform_real<> uni_dist;
    boost::variate_generator<Generator&, boost::uniform_real<> > nextVariate;
};

inline RandomUniform* randomUniform()
{
    if (!RandomUniform::_randomUniform) {
        RandomUniform::_randomUniform =
            objectPool()->find<RandomUniform*>(RandomUniform::id());
        RandomUniform::_randomUniform->setSeed(42);
    }
    return RandomUniform::_randomUniform;
}

} //namespace

#endif
