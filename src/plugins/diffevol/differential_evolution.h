/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef DIFFEVOL_DIFFERENTIAL_EVOLUTION_H
#define DIFFEVOL_DIFFERENTIAL_EVOLUTION_H

#include <QList>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/random/variate_generator.hpp>
#include <usbase/integrator.h>
#include <usbase/random.h>

namespace diffevol {
	
class DifferentialEvolution : public UniSim::Integrator
{

public:
    DifferentialEvolution(UniSim::Identifier name, QObject *parent=0);

    // standard methods
    void initialize();
    void reset();
    bool nextStep();

private:
    // parameters
    int maxSteps;
    double precision, a, b;

    // variables
    double deviation;
    bool found, passedMaxSteps;

    // random number generation
    typedef boost::random::uniform_real_distribution<double> Distribution;
    typedef boost::variate_generator<UniSim::Random::Generator&, Distribution> Variate;
    Distribution *distribution;
    Variate *variate;

    // data
    bool stopInNextStep;
    struct Sample {
        const double *area, *slope, *fertiliser, *fungicide, *yield;
    };
    QList<Sample> samples;

    // methods
    void collectSamples();
    void computeDeviation();
};


} //namespace


#endif
