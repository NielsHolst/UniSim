/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_PLOT_WIDGET
#define UNISIM_PLOT_WIDGET

class QString;
class QWidget;
class QwtPlot;
class QwtPlotCurve;

namespace UniSim {

class PlotWidget {
public:
    PlotWidget(QwtPlot *plot, QWidget *widget);
    void setXYtitles(QString xTitle, QString yTitle);
    void showLegend(bool doShow);
    void addCurve(QwtPlotCurve *curve);
    void show();
    QwtPlot* plot();
private:
    QwtPlot *_plot;
    QWidget *widget;
};


} //namespace

#endif
