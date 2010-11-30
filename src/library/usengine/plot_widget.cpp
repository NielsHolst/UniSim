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
#include <QMessageBox>

namespace UniSim {

PlotWidget::PlotWidget(QwtPlot *plot, QWidget *widget_) :
    _plot(plot), widget(widget_) {
}

void PlotWidget::setXYtitles(QString xTitle, QString yTitle) {
    _plot->setAxisTitle(QwtPlot::xBottom, xTitle);
    _plot->setAxisTitle(QwtPlot::yLeft, yTitle);
}

void PlotWidget::showLegend(bool doShow) {
    if (doShow) {
        _plot->insertLegend(new QwtLegend(), QwtPlot::RightLegend);
    }
}

void PlotWidget::addCurve(QwtPlotCurve *curve) {
    curve->attach(_plot);
}

void PlotWidget::show() {
    _plot->replot();
    widget->adjustSize();
    widget->show();
}

QwtPlot* PlotWidget::plot() {
    return _plot;
}


} //namespace
