/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include <usbase/test_num.h>
#include <usbase/utilities.h>
#include "distributed_delay_2d.h"

#include <iostream>

namespace UniSim {

void show(QString s, const QVector<double> &v) {
    std::cout << qPrintable(s) << ": ";
    for (int i = 0; i < v.size(); ++i)
        std::cout << v.at(i) << " ";
    std::cout << "\n";
}

DistributedDelay2D::DistributedDelay2D(const FixedParameters &p_, QObject *parent_, Policy policy_)
    : DistributedDelayBase(parent), p(p_), policy(policy_)
//! Create distributed delay from parameters
{
    x.resize(p.k1*p.k2);
    s.rates.resize(p.k1*p.k2);
    s.outflow1.resize(p.k2);
    s.outflow2.resize(p.k1);
    s.outflow1.resize(p.k2);
    s.outflow2.resize(p.k1);
}

DistributedDelay2D::DistributedDelay2D(const DistributedDelay2D &dd)
    : DistributedDelayBase(dd.parent), p(dd.p), policy(dd.policy), s(dd.s)
//! Create distributed delay as a copy of existing distributed delay
{
    x = dd.x;
    xSum = dd.xSum;
}

void DistributedDelay2D::update(const UpdateParameters &up) {
    // Determine update method
    bool hasGrowth = (up.fgr1!=1 || up.fgr2!=1);
    bool isSymmetric = (policy == Symmetric);
    if (policy==Flexible) {
        isSymmetric = !hasGrowth;
    }
    else if (isSymmetric && hasGrowth) {
        QString msg("Growth rate different from 1 not valid for symmetric update (fgr1=%1, fgr2=%2");
        throw Exception(msg.arg(up.fgr1).arg(up.fgr2), parent);
    }

    // Set flow parameters
    flowParameters[0] = computeFlowParameters(p.L1, p.k1, up.dt1, up.fgr1),
    flowParameters[1] = computeFlowParameters(p.L2, p.k2, up.dt2, up.fgr2);
    s.idt = std::max(flowParameters[0].idt, flowParameters[1].idt);
    adjust_a();
    // Adjust inflow for idt (Refactor this to avoid copying whe idt==1)
    flowParameters[0].inflow = *up.inflow1;
    flowParameters[1].inflow = *up.inflow2;
    UniSim::scale(flowParameters[0].inflow, 1./s.idt);
    UniSim::scale(flowParameters[1].inflow, 1./s.idt);

    // Update
    double totalBefore = xSum + accum(*up.inflow1) + accum(*up.inflow2);
    if (isSymmetric)
        updateSymmetric(totalBefore);
    else
        updateAsymmetric();

    // Done
    xSum = accum(x);
    s.growthRate = xSum + accum(s.outflow1) + accum(s.outflow2)- totalBefore;
}

void DistributedDelay2D::updateSymmetric(double totalBefore) {
    // Update in internal time steps
    s.rates.fill(0.);
    s.outflow1.fill(0.);
    s.outflow2.fill(0.);
    for (int i = 0; i < s.idt; ++i) {
        updateRates();
        addRates();
    }

    // Fix inaccuracy
    xSum = accum(x);
    double outTotal = accum(s.outflow1) + accum(s.outflow2);
    s.growthRate = xSum + outTotal - totalBefore;
    if (xSum > 0.) {
        double factor = 1. - s.growthRate/(xSum+outTotal);
        if (!TestNum::eq(factor,1.)) {
            UniSim::scale(x, factor);
            UniSim::scale(s.outflow1, factor);
            UniSim::scale(s.outflow2, factor);
        }
    }
}

void DistributedDelay2D::updateAsymmetric() {
    // Update in internal time steps
    s.rates.fill(0.);
    s.outflow1.fill(0.);
    s.outflow2.fill(0.);
    for (int i = 0; i < s.idt; ++i) {
        updateFromTop();
        updateFromLeft();
    }
}

DistributedDelay2D::FlowParameters DistributedDelay2D::computeFlowParameters(double L, int k, double dt, double fgr) const {
    FlowParameters p;
    double del = L*pow(fgr, -1./k);
    double atr = k*(1./L - 1./del);
    p.b = 1. + atr*del/k;
    p.idt = (int) floor(1.5 + 2.*p.b*dt*k/del);
    p.a = k/del*dt;
    return p;
}

void DistributedDelay2D::adjust_a() {
    FlowParameters *p(flowParameters);
    p[0].a /= s.idt;
    p[1].a /= s.idt;
    if (!(0.<p[0].a && p[0].a<=1.) && !(0.<p[1].a && p[1].a<=1.)) {
        QString msg = "Illegal value for flow coefficient a in DistributedDelay (a1==%1, a2=%2). "
                "Should be within ]0;1], idt=%3";
        throw Exception(msg.arg(p[0].a).arg(p[1].a).arg(s.idt));
    }
}

inline double attr1(const DistributedDelay2D::FlowParameters &p1, double val) {
    return p1.a*p1.b*val;
}

inline double attr2(const DistributedDelay2D::FlowParameters &p2, double val) {
    return p2.a*p2.b*val;
}

void DistributedDelay2D::updateRates() {
    FlowParameters &p1(flowParameters[0]), &p2(flowParameters[1]);
    QVector<double> &in1(p1.inflow), &in2(p2.inflow);
    double top, left;
    int i = 0, j = 0;

    // Upper left
    top = in1.at(j);
    left = in2.at(i);
    ref(i,j) = top + left;

    // Left column
    for (i = 1; i < p.k1-1; ++i) {
        ref(i-1,j) -= top = p1.a*val(i-1,j);
        left = in2.at(i);
        ref(i,j) = top + left;
    }

    // Upper row
    i = 0;
    for (j = 1; j < p.k2; ++j) {
        top = in1.at(j);
        ref(i,j-1) -= left = p2.a*val(i,j-1);
        ref(i,j) = top + left;
    }

    // Central
    for (i = 1; i < p.k1; ++i) {
        for (j = 1; j < p.k2; ++j) {
            ref(i-1,j) -= top = p1.a*val(i-1,j);
            ref(i,j-1) -= left = p2.a*val(i,j-1);
            ref(i,j) = top + left;
        }
    }

    // Lower outflow
    for (j = 0; j < p.k2; ++j) {
        ref(i-1,j) -= top = p1.a*val(i-1,j);
        s.outflow1[j] += top;
    }

    // Right outflow
    for (i = 0; i < p.k1; ++i) {
        ref(i,j-1) -= left = p2.a*val(i,j-1);
        s.outflow2[i] += left;
    }
}

void DistributedDelay2D::addRates() {
    for (int i = 0; i < p.k1*p.k2; ++i) {
        x[i] += s.rates[i];
        if (x[i] < 0) {
            QString msg = "Increase internal precision of DistributedDelay2D; negative content (%1)";
            throw Exception(msg.arg(x[i]));
        }
    }
}

void DistributedDelay2D::updateFromTop() {
    FlowParameters &p1(flowParameters[0]);
    int i = p.k1;

    // Lower outflow
    for (int j = 0; j < p.k2; ++j)
        s.outflow1[j] += p1.a*val(i-1,j);

    // Step backwards through rows
    for (i = p.k1-1; i > 0; i--)
        for (int j = 0; j < p.k2; ++j)
            xref(i,j) += p1.a*(val(i-1,j) - p1.b*val(i,j));

    // Upper row
    for (int j = 0; j < p.k2; ++j)
        xref(i,j) += p1.inflow.at(j) - p1.a*p1.b*val(i,j);
}

void DistributedDelay2D::updateFromLeft() {
    FlowParameters &p2(flowParameters[1]);
    int j = p.k2;

    // Right outflow
    for (int i = 0; i < p.k1; ++i)
        s.outflow2[i] += p2.a*val(i,j-1);

    // Step backwards through columns
    for (j = p.k2-1; j > 0; j--)
        for (int i = 0; i < p.k1; ++i)
            xref(i,j) += p2.a*(val(i,j-1) - p2.b*val(i,j));

    // Left column
    for (int i = 0; i < p.k1; ++i)
        xref(i,j) += p2.inflow.at(i) - p2.a*p2.b*val(i,j);
}

const DistributedDelay2D::State& DistributedDelay2D::state() const {
    return s;
}

int DistributedDelay2D::ix(int i, int j) const {
    bool ok = i<p.k1 && j<p.k2;
    if (!ok)
        throw Exception("ERROR");
    Q_ASSERT(i<p.k1 && j<p.k2);
    return j*p.k1 + i;
}

} // namespace
