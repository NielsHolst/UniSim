/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_RANDOM_UNIFORM
#define UNISIM_RANDOM_UNIFORM
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <QObject>
#include <usbase/random_generator.h>
#include "random_base.h"

namespace UniSim{

class RandomUniform : public RandomBase
{
	Q_OBJECT
public: 
    RandomUniform(UniSim::Identifier name, QObject *parent=0);
    ~RandomUniform();
    // standard methods
    void initialize();
private:
    // methods
    double drawValue();
    // random number generation
    typedef boost::uniform_real<double> Distribution;
    typedef boost::variate_generator<RandomGenerator::Generator&, Distribution> Variate;
    Distribution *distribution;
    Variate *variate;
};

} //namespace
#endif
