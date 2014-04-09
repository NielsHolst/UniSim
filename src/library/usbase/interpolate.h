/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_INTERPOLATE_H
#define UNISIM_INTERPOLATE_H

#include <QMap>
#include "data_grid.h"
#include "exception.h"

namespace UniSim {

template<class X, class Y> QPair<int,int> bracket(const QVector<X> increasing, X x);
template<class X, class Y, class Z> double interpolate(QPair<X,Y> p1, QPair<X,Y> p2, Z x);
template<class X, class Y> double interpolate(const QMap<X,Y> &xy, X x);
template<class X> double interpolate(const DataGrid &matrix, X rowValue, X colValue);


template<class X, class Y> QPair<int,int> bracket(const QVector<X> increasing, Y x) {
    if (increasing.isEmpty()) {
        QString msg{"Cannot bracket value '%1' in empty vector"};
        throw Exception(msg.arg(x));
    }

    auto p1 = increasing.constBegin();
    if (x <= *p1 || increasing.size() == 1) return qMakePair(0,0);

    int end = increasing.size() - 1;
    if (x >= increasing.at(end)) return qMakePair(end,end);

    auto p2 = p1; ++p2;
    int i = 0;
    while (x > *p2) { ++p1; ++p2; ++i; }
    return qMakePair(i, i+1);
}


//! Interpolate between two points
/*! Interpolates a y-value from the given x-value. Extrapolates if x is outside the point interval.
 */
template<class X, class Y, class Z>
double interpolate(QPair<X,Y> p1, QPair<X,Y> p2, Z x) {
    if (p2.first == p1.first) {
        QString msg{"Cannot interpolate between two points with same x: (%1,%2) and (%3,%4)"};
        throw Exception(msg.arg(p1.first).arg(p1.second).arg(p2.first).arg(p2.second));
    }
    return double(x - p1.first)/(p2.first - p1.first)*(p2.second - p1.second) + p1.second;

}

//! Interpolate from (x,y) table
/*! Interpolates a y-value from the given x-value. For x-values less than the first x-value in the table,
the first y-value is returned. For x-values greater then the last x-value in the table, the last y-value
is returned.
*/
template<class X, class Y>
double interpolate(const QMap<X,Y> &xy, X x) {
    if (xy.isEmpty()) {
        QString msg{"Cannot interpolate value '%1' from empty (x,y)-table"};
        throw Exception(msg.arg(x));
    }
    Y lastY = xy[0];
    auto low = xy.begin();
    for (auto high = xy.constBegin(); high != xy.constEnd(); ++high) {
        if (x < high.key()) {
            if (high.key() == low.key()) return low.value();
            return interpolate(qMakePair(low.key(), low.value()), qMakePair(high.key(), high.value()), x);
        }
        lastY = high.value();
        low = high;
    }
    return lastY;
}

//! Interpolate from (x,y) cross table with z values
template<class X> double interpolate(const DataGrid &matrix, X rowValue, X colValue) {
    if (matrix.rowNumber() < 2 || matrix.columnNumber() < 3) {
        QString msg{"Matrix (%1x%2) must at least be 2x2 to perform interpolation"};
        throw Exception(msg.arg(matrix.rowNumber()).arg(matrix.columnNumber()-1));
    }
    X x{rowValue}, y{colValue};
    QStringList xs = matrix.rowNames(), ys = matrix.columnNames();
    ys[0] = ys[1]; // remove dummy column name
    QVector<double> xIncreasing, yIncreasing;
    for (QString s : xs) {
        bool ok;
        xIncreasing << s.toDouble(&ok);
        if (!ok) {
            QString msg{"Cannot interpolate. This is not a number: '%1'"};
            throw Exception(msg.arg(s));
        }
    }
    for (QString s : ys) {
        bool ok;
        yIncreasing << s.toDouble(&ok);
        if (!ok) {
            QString msg{"Cannot interpolate. This is not a number: '%1'"};
            throw Exception(msg.arg(s));
        }
    }

    QPair<int,int> xb = bracket(xIncreasing, x),
            yb = bracket(yIncreasing, y);
    double x1 = xIncreasing.at(xb.first),
           x2 = xIncreasing.at(xb.second),
           y1 = yIncreasing.at(yb.first),
           y2 = yIncreasing.at(yb.second),
           z11 = matrix.cell<double>(xb.first, yb.first),
           z21 = matrix.cell<double>(xb.second, yb.first),
           z12 = matrix.cell<double>(xb.first, yb.second),
           z22 = matrix.cell<double>(xb.second, yb.second);
    double r1 = interpolate(qMakePair(x1, z11), qMakePair(x2, z21), x),
           r2 = interpolate(qMakePair(x1, z12), qMakePair(x2, z22), x);
    return interpolate(qMakePair(y1, r1), qMakePair(y2, r2), y);
}


} //namespace

#endif
