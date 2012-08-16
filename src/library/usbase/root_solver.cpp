/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <gsl/gsl_errno.h>
#include <QtAlgorithms>
#include "root_solver.h"
#include "exception.h"

namespace UniSim{


RootSolver::RootSolver(Problem problem)
    : maxIterations(problem.maxIterations), epsAbs(problem.epsAbs), epsRel(problem.epsRel)

{
    create();
    setup(problem);
}

void RootSolver::create() {
    gsl_set_error_handler_off();
    const gsl_root_fsolver_type *T = gsl_root_fsolver_brent;
    s = gsl_root_fsolver_alloc(T);
    if (!s) {
        throw Exception("GSL Root Solver could not be created");
    }
}

void RootSolver::setup(Problem problem) {
    gslFunction.function = problem.function;
    gslFunction.params = problem.parameters;
    if (problem.x.lower > problem.x.upper)
        qSwap(problem.x.lower, problem.x.upper);
    int retCode = gsl_root_fsolver_set(s, &gslFunction, problem.x.lower, problem.x.upper);
    if (retCode==GSL_EINVAL) {
        throw Exception("GSL Root Solver could not be set up");
    }
}

RootSolver::~RootSolver() {
    gsl_root_fsolver_free(s);
}


void RootSolver::solve(bool keepProgress) {
    solutions.clear();
    iterations = 0;
    do {
        iterations++;
        status = gsl_root_fsolver_iterate(s);
        if (status == GSL_EBADFUNC || status == GSL_EZERODIV)
            break;
        double xLower = gsl_root_fsolver_x_lower(s),
               xUpper = gsl_root_fsolver_x_upper(s);
        status = gsl_root_test_interval (xLower, xUpper, epsAbs, epsRel);
        if (status == GSL_SUCCESS)
            break;
    } while (iterations < maxIterations);
    if (keepProgress)
        solutions << solution();
}

RootSolver::Solution RootSolver::solution() {
    Solution sol;
    sol.x.lower = gsl_root_fsolver_x_lower(s);
    sol.x.upper = gsl_root_fsolver_x_upper(s);
    sol.root = gsl_root_fsolver_root(s);
    sol.iterations = iterations;
    sol.converged = (status == GSL_SUCCESS);
    sol.statusCode = status;
    return sol;
}

QString RootSolver::solutionReport() {
    Solution sol = solution();
    QString msg = sol.converged ? "Root Solver succeeded" : "Root Solver did not converge";
    msg += ":\nxLow = %1, xMin = %2, xUpp = %3, iter = %4, status code = %5";
    return msg.
            arg(sol.x.lower).
            arg(sol.root).
            arg(sol.x.upper).
            arg(sol.iterations).
            arg(sol.statusCode);
}

const RootSolver::Solutions& RootSolver::progress() {
    return solutions;
}


} //namespace
