/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_MINIMISER
#define UNISIM_MINIMISER
#include <QVector>
#include <gsl/gsl_min.h>

namespace UniSim{

class Minimiser {
public: 
    typedef double (* Function) (double x, void *ignore);
    struct Bracketing {
        double lower, minimum, upper;
    };

    struct Problem {
        Bracketing x;
        Function function;
        int maxIterations;
        double epsAbs, epsRel;
    };

    struct Values {
        Bracketing f;
    };

    struct Solution {
        Bracketing x, f;
        int iterations, statusCode;
        bool converged;
    };
    typedef QVector<Solution> Solutions;

    Minimiser(Problem problem);
    Minimiser(Problem problem, Values values);
    virtual ~Minimiser();
    void solve(bool keepProgress = false);
    Solution solution();
    QString solutionReport();
    const Solutions& progress();

private:
    // methods
    void create();
    void setup(Problem problem);
    void setup(Problem problem, Values values);
    // data
    gsl_min_fminimizer *s;
    gsl_function gslFunction;
    int iterations, maxIterations, status;
    double epsAbs, epsRel;
    Solutions solutions;
};



} //namespace
#endif
