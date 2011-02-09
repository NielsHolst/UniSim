/*
** Implementation of Comparing Floats: How To Determine if Floating Quantities Are Close Enough 
** Once a Tolerance Has Been Reached. By Alberto Squassabia. Application Development Trends, 16 March 2000.
** [http://adtmag.com/articles/2000/03/16/comparing-floats-how-to-determine-if-floating-
** quantities-are-close-enough-once-a-tolerance-has-been.aspx]
*/
#ifndef TEST_NUM_H
#define TEST_NUM_H

#include <float.h>
#include <cmath>

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
	return !eq(n,d);
}

inline bool lt(double n, double d, double feps = 1e-6) {
	return n<d && !eq(n,d);
}

inline bool le(double n, double d, double feps = 1e-6) {
	return n<d || eq(n,d);
}

inline bool gt(double n, double d, double feps = 1e-6) {
	return n>d && !eq(n,d);
}

inline bool ge(double n, double d, double feps = 1e-6) {
	return n>d || eq(n,d);
}

// Fuzzy comparison of one number with zero

inline bool neZero(double n, double feps = 1e-6) {
	return !eqZero(n);
}

inline bool ltZero(double n, double feps = 1e-6) {
    return n<0. && !eqZero(n);
}

inline bool leZero(double n, double feps = 1e-6) {
    return n<0. || eqZero(n);
}

inline bool gtZero(double n, double feps = 1e-6) {
    return n>0. && !eqZero(n);
}

inline bool geZero(double n, double feps = 1e-6) {
    return n>0. || eqZero(n);
}

} // namespace

#endif

