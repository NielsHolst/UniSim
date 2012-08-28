/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
/* Implementation of Comparing Floats: How To Determine if Floating Quantities Are Close Enough
** Once a Tolerance Has Been Reached. By Alberto Squassabia. Application Development Trends, 16 March 2000.
** [http://adtmag.com/articles/2000/03/16/comparing-floats-how-to-determine-if-floating-
** quantities-are-close-enough-once-a-tolerance-has-been.aspx]
*/
#ifndef TEST_NUM_H
#define TEST_NUM_H

#include <float.h>
#include <cmath>
#include <QObject>
#include "exception.h"

namespace TestNum {

// Fuzzy equality of one number with zero

inline bool eqZero(double n, double feps=1e-6) {
    return -feps<n && n<feps;
}

// Fuzzy comparison of two double numbers

inline bool eq(double n, double d, double feps = 1e-6) {
    if (n==0.)
        return eqZero(d, feps);
    if (d==0.)
        return eqZero(n, feps);
    if (fabs(d)<1 && fabs(n)>fabs(d)*DBL_MAX) // overflow
		return false;
    if (fabs(d)>1 && fabs(n)<fabs(d)*DBL_MIN) // underflow
        return false;
    return 1.-feps<n/d && n/d<1+feps;
}

inline bool ne(double n, double d, double feps = 1e-6) {
    return !eq(n,d,feps);
}

inline bool lt(double n, double d, double feps = 1e-6) {
    return n<d && !eq(n,d,feps);
}

inline bool le(double n, double d, double feps = 1e-6) {
    return n<d || eq(n,d,feps);
}

inline bool gt(double n, double d, double feps = 1e-6) {
    return n>d && !eq(n,d,feps);
}

inline bool ge(double n, double d, double feps = 1e-6) {
    return n>d || eq(n,d,feps);
}

// Fuzzy comparison of one number with zero

inline bool neZero(double n, double feps = 1e-6) {
    return !eqZero(n,feps);
}

inline bool ltZero(double n, double feps = 1e-6) {
    return n<0. && !eqZero(n,feps);
}

inline bool leZero(double n, double feps = 1e-6) {
    return n<0. || eqZero(n,feps);
}

inline bool gtZero(double n, double feps = 1e-6) {
    return n>0. && !eqZero(n,feps);
}

inline bool geZero(double n, double feps = 1e-6) {
    return n>0. || eqZero(n,feps);
}

// Fuzzy rounding in zero neighbourhood

inline void snapToZero(double &n, double feps = 1e-6) {
    if (eqZero(n, feps))
        n = 0.;
}

inline void assureEqZero(double &n, const char *varName, QObject *context, double feps = 1e-6) {
    if (eqZero(n, feps))
        n = 0.;
    if (n != 0)
        throw UniSim::Exception("Cannot round number to zero: " + QString(varName) + " = " + QString::number(n), context);
}

inline void assureGeZero(double &n, const char *varName, QObject *context, double feps = 1e-6) {
    if (geZero(n, feps) && n < 0.)
        n = 0.;
    if (n < 0)
        throw UniSim::Exception("Cannot round number to zero: " + QString(varName) + " = " + QString::number(n), context);
}

inline void assureLeZero(double &n, const char *varName, QObject *context, double feps = 1e-6) {
    if (leZero(n, feps) && n > 0.)
        n = 0.;
    if (n > 0)
        throw UniSim::Exception("Cannot round number to zero: " + QString(varName) + " = " + QString::number(n), context);
}

// Fuzzy rounding in neighbourhood of constant

inline void snapTo(double &n, double d, double feps = 1e-6) {
    if (eq(n, d, feps))
        n = d;
}

inline void assureEq(double &n, double d, const char *varName, QObject *context, double feps = 1e-6) {
    if (eq(n, d, feps))
        n = d;
    if (n != d)
        throw UniSim::Exception("Cannot round number to constant: " + QString(varName) +
                                " = " + QString::number(n) +
                                " != " + QString::number(d), context);
}

inline void assureGe(double &n, double d, const char *varName, QObject *context, double feps = 1e-6) {
    if (ge(n, d, feps) && n < d)
        n = d;
    if (n < d)
        throw UniSim::Exception("Cannot round number to constant: " + QString(varName) +
                                " = " + QString::number(n) +
                                " != " + QString::number(d), context);
}

inline void assureLe(double &n, double d, const char *varName, QObject *context, double feps = 1e-6) {
    if (le(n, d, feps) && n > d)
        n = d;
    if (n > d)
        throw UniSim::Exception("Cannot round number to constant: " + QString(varName) +
                                " = " + QString::number(n) +
                                " != " + QString::number(d), context);
}

} // namespace

#endif

