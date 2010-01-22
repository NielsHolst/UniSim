/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QWidget>
#include <qwt_legend.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include "plot_widget.h"

namespace UniSim {

PlotWidget::PlotWidget(QwtPlot *plot_, QWidget *widget_) :
    plot(plot_), widget(widget_) {
}

void PlotWidget::setXYtitles(QString xTitle, QString yTitle) {
    plot->setAxisTitle(QwtPlot::xBottom, xTitle);
    plot->setAxisTitle(QwtPlot::yLeft, yTitle);
}

void PlotWidget::showLegend(bool doShow) {
    if (doShow)
        plot->insertLegend(new QwtLegend(), QwtPlot::RightLegend);
}

void PlotWidget::addCurve(QwtPlotCurve *curve) {
    curve->attach(plot);
}

void PlotWidget::show() {
    plot->replot();
    widget->adjustSize();
    widget->show();
}

} //namespace
