/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_RANDOM_H
#define UNISIM_RANDOM_H
#include <QObject>
#include <boost/random/mersenne_twister.hpp>
#include <usbase/object_pool.h>

namespace UniSim{

class Random : public QObject
{
	Q_OBJECT
public: 
    typedef boost::mt19937 Generator;
    Random();
    static QString id();
private:
    Generator generator;
    // Singleton
    static Random *theRandomGenerator;
    friend Random::Generator* randomGenerator();

};

inline Random::Generator* randomGenerator()
{
    if (!Random::theRandomGenerator)
        Random::theRandomGenerator = objectPool()->find<Random*>(Random::id());
    return &(Random::theRandomGenerator->generator);
}

} //namespace
#endif
