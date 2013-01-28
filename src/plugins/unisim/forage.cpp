/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <usbase/test_num.h>
#include <usbase/utilities.h>
#include "forage.h"

namespace UniSim{
namespace Forage{

QVector<double> parts(const QVector<double> &proportionsWanted) {
    int n = proportionsWanted.size();
    const double *p = proportionsWanted.data();
    QVector<double> u = QVector<double>(1, *p);
    if (n == 1)
        return u;
    ++p;
    for (int i = 1; i < n; ++i, ++p) {
        QVector<double> v;
        int m = u.size();
        const double *q = u.data();
        double uSum = 0.;
        for (int j = 0; j < m; ++j, ++q) {
            double x1 = *q*(1.-*p);
            double x2 = *q*(*p);
            v << x1 << x2;
            uSum += *q;
        }
        v << *p*(1.-uSum);
        u = v;
    }
    return u;
}

namespace {

    int iPow(int x, int n) {
        Q_ASSERT(n>=0);
        if (n == 0)
            return 1;
        int res = x;
        for (int i = 1; i < n; ++i)
            res *= x;
        return res;
    }
}

QVector<int> firstLevel(int n) {
    if (n == 1)
        return QVector<int>(1, 0);

    int m0 = iPow(2, n-1) - 1;
    QVector<int> v(m0);
    for (int i = 0; i < m0; ++i) {
        v[i] = 2*(i+1) - 1;
    }
    v << iPow(2, n) - 2;
    return v;
}

QVector<int> finalLevel(int from, int to) {
    QVector<int> prev = firstLevel(from);
    if (from == to)
        return prev;
    for (int i = from; i < to; ++i) {
        QVector<int> next;
        for (int k = 0; k < prev.size(); ++k) {
            next << 2*prev[k] << 2*prev[k]+1;
        }
        prev = next;
    }
    return prev;
}

Matrix<double> owners(int n) {
    Matrix<double> M;
    int numColumns = iPow(2,n) - 1;
    M.resize(n, numColumns);
    for (int i = 1; i <= n; ++i) {
        QVector<int> positives = finalLevel(i,n);
        for (int j = 0; j < positives.size(); ++j) {
            M(i-1,positives.at(j)) = 1;
        }
    }
    return M;
}

Matrix<double> weights(int n) {
    Matrix<double> W = owners(n);
    int numRows = n;
    int numCols = W.numColumns();
    for (int j = 0; j < numCols; ++j) {
		int colSum = 0;
        for (int i = 0; i < numRows; ++i)
			colSum += W.at(i,j);
        for (int i = 0; i < numRows; ++i)
            W(i,j) /= colSum;
	}
    return W;
}

QVector<double> forage(const QVector<double> &proportionsWanted) {
    int n = proportionsWanted.size();
    Matrix<double> W = weights(n);
    QVector<double> P = parts(proportionsWanted);
    int m = P.size();
    QVector<double> R(n);
    for (int consumer = 0; consumer < n; ++consumer) {
        double rowProductSum = 0;
        for (int j = 0; j < m; ++j) {
            rowProductSum += P.at(j)*W.at(consumer,j);
        }
        R[consumer] = rowProductSum;
    }
    Q_ASSERT(TestNum::le(accum(R),1.));
    return R;
}


} //namespace
} //namespace

