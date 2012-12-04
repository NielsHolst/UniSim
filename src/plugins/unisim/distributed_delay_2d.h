/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_DISTRIBUTED_DELAY_2D_H
#define UNISIM_DISTRIBUTED_DELAY_2D_H
#include <QObject>
#include "distributed_delay_base.h"

namespace UniSim {

class DistributedDelay2D : public DistributedDelayBase
{
public:
    struct FixedParameters {
        double L1, L2;
        int k1, k2, minIter;
    };
    struct UpdateParameters {
        double dt1, dt2, fgr1, fgr2;
        const QVector<double> *inflow1, *inflow2;
        UpdateParameters() : dt1(0), dt2(0), fgr1(1), fgr2(1), inflow1(0), inflow2(0) {}
    };
    struct State {
        double growthRate;
        QVector<double> rates;          // dimension k1*k2
        QVector<double> outflow1;       // dimension k2
        QVector<double> outflow2;       // dimension k1
        int idt;
        State() : growthRate(0) {}
    };
    struct FlowParameters {
        double a, b;
        int idt;
        QVector<double> inflow;
    };
    typedef enum {Flexible, Symmetric, Asymmetric} Policy;

    DistributedDelay2D(const FixedParameters &p, QObject *parent, Policy policy = Flexible);
    DistributedDelay2D(const DistributedDelay2D &dd);
    void update(const UpdateParameters &up);
    const State& state() const;

    int ix(int i, int j) const;
    inline double val(int i, int j) const;
    inline double& ref(int i, int j);
    inline double& xref(int i, int j);

private:
    // data
    FixedParameters p;
    Policy policy;
    FlowParameters flowParameters[2];
    State s;
    // methods
    void updateFromTop();
    void updateFromLeft();
    void updateSymmetric(double totalBefore);
    void updateAsymmetric();
    void updateRates();
    void addRates();
    FlowParameters computeFlowParameters(double L, int k, double dt, double fgr) const;
    void adjust_a();

};


inline double DistributedDelay2D::val(int i, int j) const {
    return x.at(ix(i,j));
}

inline double& DistributedDelay2D::ref(int i, int j) {
    return s.rates[ix(i,j)];
}

inline double& DistributedDelay2D::xref(int i, int j) {
    return x[ix(i,j)];
}

}
#endif
