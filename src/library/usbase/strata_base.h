/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_STRATA_BASE_H
#define UNISIM_STRATA_BASE_H
#include <cmath>
#include <QVector>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <QDate>
#include <QTime>
#include <usbase/exception.h>
#include <usbase/object_pool.h>
#include <usbase/random_generator.h>

namespace UniSim{

class StrataBase
{
public:
    enum Type {Absolute, Factor, Relative};
    StrataBase(Type type_) : type(type_) {}
protected:
    virtual void stratify() = 0;
    Type type;
    // random number generation
    RandomGenerator *generator;
    typedef boost::uniform_real<double> Distribution;
    typedef boost::variate_generator<RandomGenerator::Generator&, Distribution> Variate;
    Distribution *distribution;
    Variate *variate;
    double u() { return (*variate)(); }
};

} //namespace

#endif

