/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_STRATA_H
#define UNISIM_STRATA_H
#include <cmath>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <QDate>
#include <QTime>
#include <QVector>
#include <usbase/exception.h>
#include <usbase/object_pool.h>
#include <usbase/random.h>
#include "strata_base.h"

namespace UniSim{

template <class T>
class Strata : public StrataBase
{
public:
    Strata(T value, double deviance, int n, Type type);
    ~Strata();
    T stratum(int i);
    T sample(int i);
    const QVector<T>* strata() const;
    const QVector<T>* samples() const;
    T nextSample();
private:
    void stratify();
    void stratifyAbsolute();
    void stratifyFactor();
    void stratifyRelative();

    T value;
    double deviance;

    int n;
    QVector<T> _strata, _samples;
    int current;
};

namespace {
    int f(double x) { return (int) floor(x + 0.5); }

    template <class T, class U> T add(T x, U a) { return x + a;  }
    template <> int add<int, double>(int x, double a) { return x + f(a); }
    template <> QDate add<QDate,int>(QDate x, int a) { return x.addDays(a); }
    template <> QTime add<QTime,int>(QTime x, int a) { return x.addSecs(a); }
    template <> QDate add<QDate,double>(QDate x, double a) { return x.addDays(f(a)); }
    template <> QTime add<QTime,double>(QTime x, double a) { return x.addSecs(f(a)); }

    template <class T> double toDouble(T x) { return x; }
    template <> double toDouble<QString>(QString) { return 0; }
    template <> double toDouble<QDate>(QDate) { return 0; }
    template <> double toDouble<QTime>(QTime) { return 0; }

    template <class T> T fromDouble(double x) { return x; }
    template <> int fromDouble(double x) { return f(x); }
    template <> QString fromDouble<QString>(double) { return QString(); }
    template <> QDate fromDouble<QDate>(double) { return QDate(); }
    template <> QTime fromDouble<QTime>(double) { return QTime(); }
}

template <class T>
Strata<T>::Strata(T value_, double deviance_, int n_, StrataBase::Type type)
    : StrataBase(type), value(value_), deviance(deviance_), n(n_)
{
    distribution = new Distribution(0., 1.);
    variate = new Variate(*randomGenerator(), *distribution);
    stratify();
}

template <class T>
Strata<T>::~Strata()
{
    delete distribution;
    delete variate;
}

template <class T>
void Strata<T>::stratify()
{
    switch (type) {
    case Absolute:
        stratifyAbsolute();
        break;
    case Factor:
        stratifyFactor();
        break;
    case Relative:
        stratifyRelative();
    }
    current = 0;
}

template <class T>
void Strata<T>::stratifyAbsolute()
{
    _strata.resize(n+1);
    _samples.resize(n);
    T xmin = add(value, -deviance);
    double dx = 2.*deviance/n;
    for (int i = 0; i <= n; ++i) {
        _strata[i] = add(xmin, dx*i);
        if (i==n) break;
        _samples[i] = add(_strata[i], dx*u());
    }
}

template <class T>
void Strata<T>::stratifyFactor()
{
    if (deviance<=0) {
        QString s = QString("The factor for value stratification must be positive not %1").arg(deviance);
        throw Exception(s);
    }
    _strata.resize(n+1);
    _samples.resize(n);
    double xmin = log(toDouble(value)/deviance);
    double dx = 2.*log(deviance)/n;
    for (int i = 0; i <= n; ++i) {
        double st = xmin + dx*i;
        _strata[i] = fromDouble<T>(exp(st));
        if (i==n) break;
        double sa = st + dx*u();
        _samples[i] = fromDouble<T>(exp(sa));
    }
}

template <class T>
void Strata<T>::stratifyRelative()
{
    if (deviance<=0 || deviance>1) {
        QString s = QString("The ratio for value stratification must be in the interval ]0;1] not %1").arg(deviance);
        throw Exception(s);
    }
    _strata.resize(n+1);
    _samples.resize(n);
    double xmin = toDouble(value)*(1. - deviance);
    double dx = 2.*deviance*toDouble(value)/n;
    for (int i = 0; i <= n; ++i) {
        double st = xmin + dx*i;
        _strata[i] = fromDouble<T>(st);
        if (i==n) break;
        double sa = st + dx*u();
        _samples[i] = fromDouble<T>(sa);
    }
}

template <class T>
T Strata<T>::stratum(int i)
{
    Q_ASSERT(i < n+1);
    return _strata[i];
}

template <class T>
T Strata<T>::sample(int i)
{
    Q_ASSERT(i < n);
    return _samples[i];
}

template <class T>
const QVector<T>* Strata<T>::strata() const {
    return &_strata;
}

template <class T>
const QVector<T>* Strata<T>::samples() const {
    return &_samples;
}

template <class T>
T Strata<T>::nextSample() {
    if (current >= n) {
        QString s = QString("T Strata<T>::nextSample().\nIndex out of bounds: %1>=%2").arg(current).arg(n);
        throw Exception(s);
    }
    return sample(current++);
}


} //namespace

#endif

