/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <gsl/gsl_errno.h>
#include "minimiser.h"
#include "exception.h"

namespace UniSim{


Minimiser::Minimiser(Problem problem)
    : maxIterations(problem.maxIterations), epsAbs(problem.epsAbs), epsRel(problem.epsRel)

{
    create();
    setup(problem);
}

void Minimiser::create() {
    gsl_set_error_handler_off();
    const gsl_min_fminimizer_type *T = gsl_min_fminimizer_brent;
    s = gsl_min_fminimizer_alloc(T);
    if (!s) {
        throw Exception("GSL Minimiser could not be created");
    }
}

void Minimiser::setup(Problem problem) {
    gslFunction.function = problem.function;
    gslFunction.params = 0;
    int retCode = gsl_min_fminimizer_set(s, &gslFunction, problem.x.minimum, problem.x.lower, problem.x.upper);
    if (retCode==GSL_EINVAL) {
        throw Exception("GSL Minimiser could not be set up");
    }
}

Minimiser::Minimiser(Problem problem, Values values)
    : maxIterations(problem.maxIterations), epsAbs(problem.epsAbs), epsRel(problem.epsRel)
{
    create();
    setup(problem, values);

}

void Minimiser::setup(Problem problem, Values values) {
    gslFunction.function = problem.function;
    gslFunction.params = 0;
    int retCode = gsl_min_fminimizer_set_with_values(s, &gslFunction,
                                                     problem.x.minimum, values.f.minimum,
                                                     problem.x.lower, values.f.lower,
                                                     problem.x.upper, values.f.upper);
    if (retCode==GSL_EINVAL) {
        throw Exception("GSL Minimiser could not be set up");
    }
}

Minimiser::~Minimiser() {
    gsl_min_fminimizer_free(s);
}



void Minimiser::solve(bool keepProgress) {
    solutions.clear();
    iterations = 0;
    do {
        iterations++;
        status = gsl_min_fminimizer_iterate(s);
        if (status == GSL_EBADFUNC || status == GSL_FAILURE)
            break;
        double xLower = gsl_min_fminimizer_x_lower(s),
               xUpper = gsl_min_fminimizer_x_upper(s);
        status = gsl_min_test_interval (xLower, xUpper, epsAbs, epsRel);
        if (status == GSL_SUCCESS)
            break;
    } while (iterations < maxIterations);
    if (keepProgress)
        solutions << solution();
}

Minimiser::Solution Minimiser::solution() {
    Solution sol;
    sol.x.lower = gsl_min_fminimizer_x_lower(s);
    sol.x.upper = gsl_min_fminimizer_x_upper(s);
    sol.x.minimum = gsl_min_fminimizer_x_minimum(s);
    sol.f.lower = gsl_min_fminimizer_f_lower(s);
    sol.f.upper = gsl_min_fminimizer_f_upper(s);
    sol.f.minimum = gsl_min_fminimizer_f_minimum(s);
    sol.iterations = iterations;
    sol.converged = (status == GSL_SUCCESS);
    sol.statusCode = status;
    return sol;
}

QString Minimiser::solutionReport() {
    Solution sol = solution();
    QString msg = sol.converged ? "Minimiser succeeded" : "Minimiser did not converge";
    msg += ":\nxLow = %1, xMin = %2, xUpp = %3, fLow = %4, fMin = %5, fUpp = %6, iter = %7, status code = %8";
    return msg.
            arg(sol.x.lower).
            arg(sol.x.minimum).
            arg(sol.x.upper).
            arg(sol.f.lower).
            arg(sol.f.minimum).
            arg(sol.f.upper).
            arg(sol.iterations).
            arg(sol.statusCode);
}

const Minimiser::Solutions& Minimiser::progress() {
    return solutions;
}


} //namespace
