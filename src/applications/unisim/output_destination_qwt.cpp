/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <QColor>
#include <QFile>
#include <QRgb>
#include <QTextStream>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <usengine/xy_state_variables.h>
#include "output_destination_qwt.h"
#include "sub_window.h"

namespace UniSim{

int OutputDestinationQwt::_count = 0;
QList<QColor> OutputDestinationQwt::_colors;

OutputDestinationQwt::OutputDestinationQwt(QwtPlot *plot, SubWindow *subWindow,Output *output)
    : OutputDestination("output_qwt_"+QString::number(++_count), output),
    _plot(plot), _subWindow(subWindow)
{
    Q_ASSERT(_plot);
    Q_ASSERT(_subWindow);
    _colors << QColor("#FF416E")
            << QColor("#3760D5")
            << QColor("#FFC741")
            << QColor("#34C1E8")
            << QColor("#F7FF41")
            << QColor("#A135D0")
            << QColor("#39DE52");
}

void OutputDestinationQwt::showTitle() {
}

void OutputDestinationQwt::showLabels() {
}

void OutputDestinationQwt::showData() {

    int curveCount = 0;
    _plot->clear();
    for (QList<XYStateVariables*>::const_iterator xyi = _xyList->begin(); xyi != _xyList->end(); ++xyi) {
        XYStateVariables *xy = *xyi;
        const XYStateVariables::Variable &x(xy->xVariables()[0]);
        _plot->setAxisTitle(QwtPlot::xBottom, x.label);
        _plot->setAxisTitle(QwtPlot::yLeft, " ");

        QStringList yLabels = xy->uniqueYLabels();
        QString yAxisTitle = yLabels.size() == 1 ? yLabels[0] : QString(" ");
        _plot->setAxisTitle(QwtPlot::yLeft, yAxisTitle);
        
        if (yLabels.size() > 1)
            _plot->insertLegend(new QwtLegend(), QwtPlot::RightLegend);

        for (int i = 0; i < yLabels.size(); ++i, ++curveCount) {
            QwtPlotCurve *curve = new QwtPlotCurve(yLabels[i]);
            QColor color = _colors[curveCount % _colors.size()];
            QPen pen = QPen(color);
            pen.setWidth(4);
            curve->setPen(pen);

            curve->attach(_plot);

            const XYStateVariables::Variable &y(xy->yVariables()[i]);
            Q_ASSERT(x.data.size() == y.data.size());
            curve->setData(x.data.data(), y.data.data(), x.data.size());
        }
    }
    _plot->replot();
    _subWindow->adjustSize();
    _subWindow->show();
}


}
