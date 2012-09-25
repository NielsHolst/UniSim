/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include <usbase/root_solver.h>
#include <usbase/test_num.h>
#include <usbase/utilities.h>
#include "distributed_delay.h"
	
namespace UniSim {

DistributedDelay::DistributedDelay(const Parameters &p_)
    : p(p_)
//! Create distributed delay from parameters
{
    x.resize(p.k);
    s.data = x.data();
}

DistributedDelay::DistributedDelay(const DistributedDelay &dd)
    : p(dd.p), s(dd.s), x(dd.x)
//! Create distributed delay as a copy of existing distributed delay
{
}

void DistributedDelay::update(double inflow, double dt, double fgr) {
//! Update distributed delay
/*! Add inflow and update by time step (dt). Apply finite growth rate () by 'attrition' mechanism.
*/
    // To compute net change
    double totalBefore = s.content + inflow;

    // Set del and attrition according to Vansickle
    double 	del = p.L*pow(fgr, -1./p.k),
            atr = p.k*(1./p.L - 1./del);		// atr <= 0 always

    // Calculate attrition factor
    double b = 1. + atr*del/p.k;

    // Divide time step to increase precision
    int idt = (int) floor(1.5 + 2.*b*dt*p.k/del);
    if (idt < p.minIter) idt = p.minIter;

    // Correct inflow for divided time step
    double dividedInflow = inflow/idt;

    // Calculate flow coefficient
    double a = p.k/del*dt/idt;
    if (!(0.<a && a<=1.)) {
        QString msg = "Illegal value for flow coefficient in DistributedDelay (a==%1). "
                "Should be within ]0;1]. Other parameters: k=%2, del=%3, dt=%4, idt=%5, fgr=%6, L=%7";
        throw Exception(msg.arg(a).arg(p.k).arg(del).arg(dt).arg(idt).arg(fgr).arg(p.L));
    }

    // Integrate
    s.outflowRate = 0;
    for (int j = 0; j < idt; j++){
        // Collect outflow
        s.outflowRate += a*x.at(p.k-1);
        // Step backwards through age classes
        for (int i = p.k-1; i > 0; i--)
            x[i] += a*(x.at(i-1) - b*x.at(i));
        // Finish with first age class; enter inflow into that
        x[0] += dividedInflow - a*b*x.at(0);
    }
    s.data = x.data();
    s.content = accum(x);
    s.growthRate = s.content + s.outflowRate - totalBefore;
}

DistributedDelay::Brackets DistributedDelay::bracket(double inflow, double dt, double fgr, double sdRatio) {
//! Find two values of fgr that results in a growth rate that brackets the supply
/*! In case supply is zero then the brackets are (fgr1, fgr2) = (1,1)
    In case the distributed delay is empty then the brackets are (fgr1, fgr2) = (fgr,fgr)
*/
    Q_ASSERT(sdRatio>=0 && sdRatio<=1);
    Brackets b;
    b.fgr1 = b.fgr2 = fgr;
    DistributedDelay dd1(*this);
    dd1.update(inflow, dt, b.fgr1);

    double demand = b.growth1 = dd1.state().growthRate;
    double supply = sdRatio*demand;
    while (true) {
        DistributedDelay dd2(*this);
        b.fgr2 /= 2;
        if (b.fgr2 < 1.) b.fgr2 = 1.;
        dd2.update(inflow, dt, b.fgr2);
        b.growth2 = dd2.state().growthRate;
        if (b.growth2 < supply) break;
    }
    return b;
}

namespace {
    struct RootSolverParam {
        DistributedDelay *dd;
        double inflow, dt, supply;
    };

    double rootSolverFunc(double fgr, void *parameters) {
        RootSolverParam *p = (RootSolverParam *) parameters;
        DistributedDelay dd(*(p->dd));
        dd.update(p->inflow, p->dt, fgr);
        return p->supply - dd.state().growthRate;
    }
}

double DistributedDelay::findFgr(double inflow, double dt, double fgr, double sdRatio) {
//! Find a value of fgr that yields a growth rate equal to the supply
/*!
*/
    Q_ASSERT(sdRatio>=0 && sdRatio<=1);
    bool isEmpty = s.content + inflow == 0.;
    if (sdRatio == 0)
        return 1;
    else if (sdRatio == 1 || isEmpty)
        return fgr;

    DistributedDelay::Brackets b = bracket(inflow, dt, fgr, sdRatio);

    RootSolverParam p;
    p.dd = this;
    p.inflow= inflow;
    p.dt = dt;
    p.supply = b.growth1*sdRatio;

    RootSolver::Problem problem;
    problem.x.lower = b.fgr1;
    problem.x.upper = b.fgr2;
    problem.function = rootSolverFunc;
    problem.parameters = &p;
    problem.maxIterations = 100;
    problem.epsAbs = 0.001;
    problem.epsRel = 0;

    RootSolver rs(problem);
    rs.solve();
    RootSolver::Solution sol = rs.solution();
    if (!sol.converged)
        throw Exception("DistributedDelay could not estimate growth rate to match supply");
    return sol.root;
}

void DistributedDelay::scale(double factor) {
    if (factor == 0.) {
        x.fill(0.);
    }
    else if (factor != 1.) {
        double *v = x.data();
        for (int i = 0; i < p.k; ++i, ++v) *v *= factor;
    }
}

DistributedDelay::State DistributedDelay::state() const {
    return s;
}

} // namespace
