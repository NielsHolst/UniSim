/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_RANDOM_GENERATOR
#define UNISIM_RANDOM_GENERATOR
#include <QObject>
#include <boost/random/mersenne_twister.hpp>

namespace UniSim{

class RandomGenerator : public QObject
{
	Q_OBJECT
public: 
    typedef boost::mt19937 Generator;

    RandomGenerator();
    ~RandomGenerator();

    Generator* generator();
private:
    Generator *_generator;
};

} //namespace
#endif
