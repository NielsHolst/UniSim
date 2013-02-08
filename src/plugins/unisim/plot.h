/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_PLOT
#define UNISIM_PLOT

#include <QColor>
#include <QList>
#include <QPair>
#include <QPen>
#include <QString>
#include <QVector>
#include <usbase/trace.h>

class QwtSymbol;

namespace UniSim{

class PlotWidget;

struct Plot
{
    QVector<double> *x, *y;
    bool logy;
    double ymin, ymax;
    QString yLegend;
    bool showLegend;
    PlotWidget *plotWidget;
    Trace::Type type;
    QPen pen;
    QwtSymbol *symbol;

    Plot();
    void add();

    // public for testing
    typedef QPair<int, int> Interval;
    typedef QList<Interval> Intervals;
    void setIntervals();
    const Intervals& getIntervals() const;
    bool includeY(double y) const;
private:
    Intervals intervals;
};


} //namespace


#endif
