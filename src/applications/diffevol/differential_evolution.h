/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef DIFFEVOL_DIFFERENTIAL_EVOLUTION_H
#define DIFFEVOL_DIFFERENTIAL_EVOLUTION_H

#include <QFile>
#include <QList>
#include <QVector>
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
    // optimization parameters
    int maxSteps;
    double precision;
    QString outputFileName;

    // regression coefficient parameters
    struct Coefficients {
        double fert, fung, intercept;
    };
    Coefficients coeff;

    // variables
    double deviation;
    bool found, passedMaxSteps, stopInNextStep;

    // DE population data
    struct Allele {
        QString name;
        double area, slope, fertiliser, fungicide, yield;
    };
    typedef QVector<Allele> Genome;
    Genome genome;              // Only one genome present; it drifts randomly
    QList<Genome> population;   // Not yet used

    // random number generation
    typedef boost::random::uniform_real_distribution<double> Distribution;
    typedef boost::variate_generator<UniSim::Random::Generator&, Distribution> Variate;
    Distribution *distribution;
    Variate *variate;

    // methods
    void collectPlots();
    void computeDeviation();
    void drift();
    void drift(Allele &a);
    void openFile();
    void writeOutput();
};


} //namespace


#endif
