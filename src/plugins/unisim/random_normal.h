/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_RANDOM_NORMAL
#define UNISIM_RANDOM_NORMAL
#include <boost/random/normal_distribution.hpp>
#include <boost/random/variate_generator.hpp>
#include <QObject>
#include <usbase/random.h>
#include "random_base.h"

namespace UniSim{

class RandomNormal : public RandomBase
{
	//Q_OBJECT
public: 
    RandomNormal(UniSim::Identifier name, QObject *parent=0);
    ~RandomNormal();
    // standard methods
    void initialize();
private:
    // parameters
    double mean, sd;
    // methods
    double drawValue();
    // random number generation
    typedef boost::normal_distribution<double> Distribution;
    typedef boost::variate_generator<Random::Generator&, Distribution> Variate;
    Distribution *distribution;
    Variate *variate;
};

} //namespace
#endif
