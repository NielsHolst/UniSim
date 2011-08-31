/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_RANDOM_GENERATOR
#define UNISIM_RANDOM_GENERATOR
#include <QObject>
#include <boost/random/mersenne_twister.hpp>
#include <usbase/object_pool.h>

namespace UniSim{

class RandomGenerator : public QObject
{
	Q_OBJECT
public: 
    typedef boost::mt19937 Generator;

    RandomGenerator();
    ~RandomGenerator();
    static QString id();

    Generator* generator();
private:
    Generator* _generator;
    // Singleton
    static RandomGenerator *theRandomGenerator;
    friend RandomGenerator* randomGenerator();

};

inline RandomGenerator* randomGenerator()
{
    if (!RandomGenerator::theRandomGenerator)
        RandomGenerator::theRandomGenerator = objectPool()->find<RandomGenerator*>(RandomGenerator::id());
    return RandomGenerator::theRandomGenerator;
}

} //namespace
#endif
