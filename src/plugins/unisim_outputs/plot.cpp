/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

#include <float.h>
#include <cmath>
#include <qwt_plot_curve.h>
#include <usengine/plot_widget.h>
#include "plot.h"

namespace UniSim{

Plot::Plot()
    : x(0), y(0), logy(false), ymin(-DBL_MAX), ymax(DBL_MAX), showLegend(true), plotWidget(0), type(Curve)
{
}

void Plot::add() {
    Q_ASSERT(plotWidget && x && y && x->size()==y->size());
    setIntervals();
    for (int i = 0; i < intervals.size(); ++i) {
        QwtPlotCurve *curve = new QwtPlotCurve(yLegend);
        curve->setItemAttribute(QwtPlotItem::Legend, showLegend && i==0);
        plotWidget->addCurve(curve);

        if (type == Symbols) {
            curve->setStyle(QwtPlotCurve::NoCurve);
            curve->setSymbol(symbol);
        }
        else {
            curve->setPen(pen);
        }
        Interval iv = intervals[i];
        int numPoints = iv.second - iv.first + 1;
        if (numPoints <=0 )
        Q_ASSERT(numPoints > 0);
        curve->setData(x->data() + iv.first, y->data() + iv.first, numPoints);
    }

}

void Plot::setIntervals() {
    intervals.clear();
    int n = x->size();
    const double *px = x->data();
    double *py = y->data();
    bool insideInterval = false;
    Interval interval;
    for (int i = 0; i < n; ++i, ++px, ++py) {
        bool doInclude = includeY(*py);
        if (doInclude) {
            if (logy) *py = log10(*py);
            if (!insideInterval) {
                interval.first = i;
                insideInterval = true;
            }
        }
        if (!doInclude || i==n-1) {
            if (insideInterval) {
                interval.second = i==n-1 ? i : i-1;
                if (interval.first > interval.second)
                    Q_ASSERT(interval.first <= interval.second);
                intervals.append(interval);
                insideInterval = false;
            }

        }
    }
}

bool Plot::includeY(double y) const {
    if (logy && y <= 0.)
        return false;
    double ytest = logy ? log10(y) : y;
    return (ytest>=ymin && ytest<=ymax) ;
}

const Plot::Intervals& Plot::getIntervals() const {
    return intervals;
}


} //namespace



