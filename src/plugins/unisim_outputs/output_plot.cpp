/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QBrush>
#include <QMessageBox>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_zoomer.h>
#include <qwt_symbol.h>
#include <usbase/dataset.h>
#include <usbase/file_locations.h>
#include <usbase/object_pool.h>
#include <usbase/output_data.h>
#include <usbase/output_variable.h>
#include <usbase/parameter.h>
#include <usengine/main_window_interface.h>
#include <usengine/plot_widget.h>
#include "output_plot.h"

namespace UniSim{

QList<QColor> OutputPlot::colors;

OutputPlot::OutputPlot(Identifier name, QObject *parent)
    : Output(name, parent), plotWidget(0)
{
    new Parameter<QString>("title", &title, QString(), this,
                           "Title of plot shown in window top bar");

    colors
    << QColor("#FF416E")
    << QColor("#3760D5")
    << QColor("#FFC741")
    << QColor("#34C1E8")
    << QColor("#F7FF41")
    << QColor("#A135D0")
    << QColor("#39DE52");
}

OutputPlot::~OutputPlot() {
    delete plotWidget;
}

void OutputPlot::initialize() {
    Output::initialize();

    const OutputResults &xs(xResults()), &ys(yResults());
    if (ys.size() == 0) {
        QString msg = "Output plot " + id().label() + " has no y-results";
        throw Exception(msg, this);
    }
    if (xs.size() == 0) {
        QString msg = "Output plot " + id().label() + " has no x-result";
        throw Exception(msg, this);
    }
    else if (xs.size() > 1) {
        QString msg = "Output plot " + id().label() + " has more than one x-result:";
        for (int i = 0; i < xs.size(); ++ i)
            msg += "\n" + xs.at(i)->id().key();
        throw Exception(msg, this);
    }

    mainWindow = objectPool()->find<MainWindowInterface*>("mainWindow");
    Q_ASSERT(mainWindow);
}

void OutputPlot::cleanup() {
    if (runNumber() == 1)
        createPlotWidget();
    fillPlotWidget();
    showPlotWidget();
    mainWindow->tile();
}

bool OutputPlot::emptyResults() const {
    return xResults().isEmpty() || yResults().isEmpty();
}

bool OutputPlot::emptyData() const {
    return xData().isEmpty() || yData().isEmpty();
}

void OutputPlot::createPlotWidget() {
    plotWidget = mainWindow->createPlotWidget(title);
    plotWidget->showLegend(true);
    //QwtPlotCanvas *canvas = plotWidget->plot()->canvas();
    //new QwtPlotZoomer(canvas);   crash!!½
}

void OutputPlot::fillPlotWidget() {
    Q_ASSERT(plotWidget);
    fillWithResults();
    fillWithData();
}

void OutputPlot::fillWithResults() {
    if (emptyResults()) return;

    OutputResults xv, yv;
    xv = xResults();
    yv = yResults();
    Q_ASSERT(xv.size() == 1);
    OutputResult *x = xv[0];

    QString yAxisTitle(" ");
    plotWidget->setXYtitles(x->id().label(), yAxisTitle);

    for (int i = 0; i < yv.size(); ++i) {
        OutputResult *y = yv[i];
        QwtPlotCurve *curve = new QwtPlotCurve(y->id().label());
        bool showLegend = (runNumber() == 1);
        curve->setItemAttribute(QwtPlotItem::Legend, showLegend);
        plotWidget->addCurve(curve);

        QColor color = colors[i % colors.size()];
        QPen pen = QPen(color);
        int width = 2;
        pen.setWidth(width);
        curve->setPen(pen);

        int numPoints = x->history()->size();
        Q_ASSERT(numPoints == y->history()->size());
        curve->setData(x->history()->data(), y->history()->data(), numPoints);
    }
}

void OutputPlot::fillWithData() {
    if (emptyData()) return;

    QList<OutputData *> xv, yv;
    xv = xData();
    yv = yData();
    Q_ASSERT(xv.size() == 1);
    OutputData *x = xv[0];

    QString yAxisTitle(" ");
    plotWidget->setXYtitles(x->id().label(), yAxisTitle);
    plotWidget->showLegend(true);

    for (int i = 0; i < yv.size(); ++i) {
        OutputData *y = yv[i];
        QwtPlotCurve *curve = new QwtPlotCurve(y->id().label());
        plotWidget->addCurve(curve);

        QColor color = colors[i % colors.size()];

        curve->setStyle(QwtPlotCurve::NoCurve);

        QPen symPen = QPen(color);
        symPen.setWidth(2);
        QBrush brush;
        QwtSymbol symbol(QwtSymbol::Ellipse, brush, symPen, QSize(8,8));
        curve->setSymbol(symbol);

        int numPoints = x->data()->size();
        Q_ASSERT(numPoints == y->data()->size());
        curve->setData(x->data()->data(), y->data()->data(), numPoints);
    }
}

void OutputPlot::showPlotWidget() {
    Q_ASSERT(plotWidget);
    plotWidget->show();
}

} //namespace

