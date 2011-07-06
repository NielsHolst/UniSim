/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_OUTPUT_PLOT
#define UNISIM_OUTPUT_PLOT

#include <QColor>
#include <QList>
#include <QPair>
#include <QPen>
#include <QString>
#include <QVector>
#include <qwt_symbol.h>

namespace UniSim{

class PlotWidget;

struct Plot
{
    enum Type {Curve, Symbols};
    QVector<double> *x, *y;
    bool logy;
    double ymin, ymax;
    QString yLegend;
    bool showLegend;
    PlotWidget *plotWidget;
    Type type;
    QPen pen;
    QwtSymbol symbol;

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
