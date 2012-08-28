/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_ROOT_SOLVER
#define UNISIM_ROOT_SOLVER
#include <QVector>
#include <gsl/gsl_roots.h>

namespace UniSim{

class RootSolver {
public: 
    typedef double (* Function) (double x, void *parameters);
    struct Bracketing {
        double lower, upper;
    };

    struct Problem {
        Bracketing x;
        Function function;
        void *parameters;
        int maxIterations;
        double epsAbs, epsRel;
    };

    struct Values {
        Bracketing f;
    };

    struct Solution {
        Bracketing x;
        double root;
        int iterations, statusCode;
        bool converged;
    };
    typedef QVector<Solution> Solutions;

    RootSolver(Problem problem);
    virtual ~RootSolver();
    void solve(bool keepProgress = false);
    Solution solution();
    QString solutionReport();
    const Solutions& progress();

private:
    // methods
    void create();
    void setup(Problem problem);
    // data
    gsl_root_fsolver *s;
    gsl_function gslFunction;
    int iterations, maxIterations, status;
    double epsAbs, epsRel;
    Solutions solutions;
};



} //namespace
#endif
