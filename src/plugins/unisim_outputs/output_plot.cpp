/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <float.h>
#include <QBrush>
#include <QMessageBox>
#include <QPair>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_zoomer.h>
#include <qwt_scale_engine.h>
#include <qwt_symbol.h>
#include <usbase/dataset.h>
#include <usbase/file_locations.h>
#include <usbase/named_object.h>
#include <usbase/object_pool.h>
#include <usbase/output_data.h>
#include <usbase/output_variable.h>
#include <usbase/parameter.h>
#include <usbase/pull_variable_base.h>
#include <usengine/main_window_interface.h>
#include <usengine/plot_widget.h>
#include "plot.h"
#include "output_plot.h"

namespace UniSim{

QList<QColor> OutputPlot::colors;

OutputPlot::OutputPlot(Identifier name, QObject *parent)
    : Output(name, parent), plotWidget(0)
{
    new Parameter<QString>("title", &title, QString(), this,
                           "Title of plot shown in window top bar");
    new Parameter<bool>("logy", &logy, false, this,
                        "Log-transform y-axis? Log base 10 is used");
    new Parameter<double>("ymin", &ymin, -DBL_MAX, this,
                          "Minimum value displayed on y-axis. Write as log-transformed value if @F {logy}");
    new Parameter<double>("ymax", &ymax, DBL_MAX, this,
                          "Maximum value displayed on y-axis. Write as log-transformed value if @F {logy}");
    new Parameter<int>("penWidth", &penWidth, 2, this,
                          "Pen width for curves and symbols");
    new Parameter<int>("symbolSize", &symbolSize, 8, this,
                          "Size of symbols");

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
        QString msg = QString("Output plot (%1) has no y-results").arg(id().label());
        throw Exception(msg, this);
    }
    if (xs.size() == 0) {
        QString msg = QString("Output plot (%1) has no x-result").arg(id().label());
        throw Exception(msg, this);
    }
    else if (xs.size() > 1) {
        QString msg = QString("Output plot (%1) has more than one x-result:").arg(id().label());
        for (int i = 0; i < xs.size(); ++ i)
            msg += "\n" + xs.at(i)->id().key();
        throw Exception(msg, this);
    }

    mainWindow = objectPool()->find<MainWindowInterface*>("mainWindow");
    Q_ASSERT(mainWindow);
}

void OutputPlot::cleanup() {
    if (!isSummary()) {
        if (runNumber() == 1) createPlotWidget();
        showPlot();
    }
}

void OutputPlot::debrief() {
    if (isSummary()) {
        createPlotWidget();
        showPlot();
    }
}

void OutputPlot::showPlot() {
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
    // This works but zero values must be ignored somehow
    //plotWidget->plot()->setAxisScaleEngine(QwtPlot::yLeft, new QwtLog10ScaleEngine);

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
    Q_ASSERT(xResults().size() == 1);
    OutputResult *x = xResults()[0];

    QString yAxisTitle(" ");
    plotWidget->setXYtitles(x->id().label(), yAxisTitle);
    setYLabels();

    for (int i = 0; i < yResults().size(); ++i) {
        Plot p;

        p.x = x->history();
        p.y = yResults()[i]->history();
        p.yLegend = yLabels[i].label();
        p.showLegend = (runNumber() == 1);
        p.plotWidget = plotWidget;

        p.logy = logy;
        p.ymin = ymin;
        p.ymax = ymax;

        QColor color = colors[i % colors.size()];
        QPen pen = QPen(color);
        pen.setWidth(penWidth);
        p.pen = pen;
        p.type = x->isOutputSummary() ? Plot::Symbols : Plot::Curve;

        QBrush brush;
        p.symbol = QwtSymbol(QwtSymbol::Ellipse, brush, pen, QSize(symbolSize,symbolSize));
        p.add();
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
        QPen pen = QPen(color);
        pen.setWidth(2);

        QBrush brush;
        QwtSymbol symbol(QwtSymbol::Ellipse, brush, pen, QSize(8,8));
        curve->setStyle(QwtPlotCurve::NoCurve);
        curve->setSymbol(&symbol);

        int numPoints = x->data()->size();
        Q_ASSERT(numPoints == y->data()->size());
        curve->setRawSamples(x->data()->data(), y->data()->data(), numPoints);
    }
}

void OutputPlot::showPlotWidget() {
    Q_ASSERT(plotWidget);
    plotWidget->show();
}

} //namespace

