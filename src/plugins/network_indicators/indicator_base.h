/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef NETWORK_INDICATORS_IMPORT_BASE_H
#define NETWORK_INDICATORS_IMPORT_BASE_H

#include <boost/random/normal_distribution.hpp>
#include <boost/random/variate_generator.hpp>
#include <QList>
#include <QVector>
#include <usbase/model.h>
#include <usbase/random.h>

namespace network_indicators {

class IndicatorBase : public UniSim::Model
{
public:
    IndicatorBase(UniSim::Identifier name, QObject *parent);
    ~IndicatorBase();
    void amend();
    void reset();
    void update();
protected:
    double rnd(double mean);
private:
    // parameters
    double cv;

    // links
    QList<Model*> farms;
    QVector<double> farmIndicators;

    // methods
    virtual double computeIndicator(UniSim::Model *farm) = 0;

    // random number generation
    typedef boost::normal_distribution<double> Distribution;
    typedef boost::variate_generator<UniSim::Random::Generator&, Distribution> Variate;
    Distribution *distribution;
    Variate *variate;

};

} //namespace


#endif
