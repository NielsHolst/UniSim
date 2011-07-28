/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <float.h>
#include <QBrush>
#include <QMessageBox>
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
#include "curve_data.h"
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
    new Parameter<double>("ymin", &ymin, DBL_MIN, this,
                          "Minimum value displayed on y-axis. Write as log-transformed value if @F {logy}");
    new Parameter<double>("ymax", &ymax, DBL_MAX, this,
                          "Maximum value displayed on y-axis. Write as log-transformed value if @F {logy}");

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

    OutputResults xv, yv;
    xv = xResults();
    yv = yResults();
    Q_ASSERT(xv.size() == 1);
    OutputResult *x = xv[0];

    QString yAxisTitle(" ");
    plotWidget->setXYtitles(x->id().label(), yAxisTitle);
    setYLabels();

    bool symbolsOnly = x->isOutputSummary();

    for (int i = 0; i < yv.size(); ++i) {
        OutputResult *y = yv[i];
        QwtPlotCurve *curve = new QwtPlotCurve(yLabels[i].label());
        bool showLegend = (runNumber() == 1);
        curve->setItemAttribute(QwtPlotItem::Legend, showLegend);
        plotWidget->addCurve(curve);

        QColor color = colors[i % colors.size()];
        QPen pen = QPen(color);
        pen.setWidth(2);

        if (symbolsOnly) {
            QBrush brush;
            QwtSymbol symbol(QwtSymbol::Ellipse, brush, pen, QSize(8,8));
            curve->setStyle(QwtPlotCurve::NoCurve);
            curve->setSymbol(symbol);
        }
        else {
            curve->setPen(pen);
        }


        Q_ASSERT(x->history()->size() == y->history()->size());
        if (logy || ymin>DBL_MIN || ymax<DBL_MAX) {
            QVector<double> xFiltered, yFiltered;
            filter(x->history(), y->history(), &xFiltered, &yFiltered);
            int numPoints = xFiltered.size();
            curve->setData(xFiltered.data(), yFiltered.data(), numPoints);
        }
        else {
            int numPoints = x->history()->size();
            curve->setData(x->history()->data(), y->history()->data(), numPoints);
        }
    }
}

void OutputPlot::filter(const QVector<double> *x0,
            const QVector<double> *y0,
            QVector<double> *x,
            QVector<double> *y)
{
    const double *px0 = x0->data();
    const double *py0 = y0->data();
    int n = x0->size();
    x->clear();
    y->clear();
    if (logy) {
        for (int i = 0; i < n; ++i, ++px0, ++py0) {
            if (*py0>0.) {
                double logy0 = log10(*py0);
                if (logy0>=ymin && logy0<=ymax) {
                    x->append(*px0);
                    y->append(logy0);
                }
            }
        }
    }
    else {
        for (int i = 0; i < n; ++i, ++px0, ++py0) {
            if (*py0>=ymin && *py0<=ymax) {
                x->append(*px0);
                y->append(log10(*py0));
            }
        }
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

