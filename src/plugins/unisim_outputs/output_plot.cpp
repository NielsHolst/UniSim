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
#include <qwt_plot_panner.h>
#include <qwt_plot_zoomer.h>
#include <qwt_scale_engine.h>
#include <usbase/exception.h>
#include <usbase/file_locations.h>
#include <usbase/named_object.h>
#include <usbase/object_pool.h>
#include <usbase/output_variable.h>
#include <usbase/parameter.h>
#include <usbase/pull_variable_base.h>
#include <usengine/main_window_interface.h>
#include <usengine/plot_widget.h>
#include "plot.h"
#include "output_plot.h"

namespace UniSim{

QList<QColor> OutputPlot::colors;
QList<QwtSymbol> OutputPlot::symbols;

OutputPlot::OutputPlot(Identifier name, QObject *parent)
    : Output(name, parent), plotWidget(0)
{    
    new Parameter<QString>("title", &title, QString(), this,
                           "Title of plot shown in window top bar");
    new Parameter<QString>("type", &typeAsString, QString("lines"), this,
                        "Type of plot: @F {lines], @F symbols or @F {both}");
    new Parameter<bool>("logy", &logy, false, this,
                        "Log-transform y-axis? Log base 10 is used");
    new Parameter<double>("ymin", &ymin, -DBL_MAX, this,
                          "Minimum value displayed on y-axis. Write as log-transformed value if @F {logy}");
    new Parameter<double>("ymax", &ymax, DBL_MAX, this,
                          "Maximum value displayed on y-axis. Write as log-transformed value if @F {logy}");
    new Parameter<int>("penWidth", &penWidth, 2, this,
                          "Pen width for curves and symbols");
    new Parameter<int>("symbolSize", &symbolSize, 6, this,
                          "Size of symbols");	
    createDesignInfo();
}

void OutputPlot::createDesignInfo() {
    if (colors.isEmpty())
        colors
        << QColor("#FF416E")
        << QColor("#3760D5")
        << QColor("#FFC741")
        << QColor("#34C1E8")
        << QColor("#F7FF41")
        << QColor("#A135D0")
        << QColor("#39DE52");
		
    if (symbols.isEmpty()) {
        int penWidth = 2;
        int symbolSize = 6;
        for (int i = 0; i < colors.size(); ++i) {
            QPen pen = QPen(colors[i]);
            pen.setWidth(penWidth);
            QColor fill = Qt::white;
            symbols << QwtSymbol(QwtSymbol::Ellipse, QBrush(fill), pen, QSize(symbolSize,symbolSize));
        }
    }
}

OutputPlot::~OutputPlot() {
    delete plotWidget;
}

void OutputPlot::initialize() {
    Output::initialize();
    initType();

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

void OutputPlot::initType() {
    QMap<QString, Type> types;
    types["lines"] = Lines;
    types["symbols"] = Symbols;
    types["both"] = Both;
    QString s = typeAsString.toLower();
    if (!types.contains(s))
        throw Exception("Unknown type of plot: '" + typeAsString + "'");
    type = types[s];
}

void OutputPlot::cleanup() {
    if (!isSummary()) {
        if (runNumber() == 1) createPlotWidget();
        showPlot();
        setZoomer();
    }
}

void OutputPlot::debrief() {
    if (isSummary()) {
        createPlotWidget();
        showPlot();
        setZoomer();
    }
}

void OutputPlot::showPlot() {
    fillPlotWidget();
    showPlotWidget();
    mainWindow->tile();
}

void OutputPlot::setZoomer() {
    QwtPlotCanvas *canvas = plotWidget->plot()->canvas();
    QwtPlotZoomer* zoomer = new QwtPlotZoomer( canvas );
    zoomer->setRubberBandPen( QColor( Qt::black ) );
    zoomer->setTrackerPen( QColor( Qt::black ) );
    // Walk up zoom stack
    zoomer->setMousePattern( QwtEventPattern::MouseSelect3, Qt::RightButton );
    // Walk down zoom stack
    zoomer->setMousePattern( QwtEventPattern::MouseSelect6, Qt::RightButton, Qt::ShiftModifier );
}

bool OutputPlot::emptyResults() const {
    return xResults().isEmpty() || yResults().isEmpty();
}

void OutputPlot::createPlotWidget() {
    plotWidget = mainWindow->createPlotWidget(title);
    plotWidget->showLegend(true);
}

void OutputPlot::fillPlotWidget() {
    Q_ASSERT(plotWidget);
    fillWithResults();
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
        int ix = i % colors.size();

        p.x = x->history();
        p.y = yResults()[i]->history();
        p.yLegend = yLabels[i].label();
        p.showLegend = (runNumber() == 1 || x->isOutputSummary());
        p.plotWidget = plotWidget;

        p.logy = logy;
        p.ymin = ymin;
        p.ymax = ymax;

        QColor color = colors[ix];
        QPen pen = QPen(color);
        pen.setWidth(penWidth);
        p.pen = pen;
        p.symbol = new QwtSymbol(QwtSymbol::Ellipse, QBrush(), pen, QSize(symbolSize,symbolSize));
        p.type = (x->isOutputSummary() || type==Symbols) ? Plot::Symbols : Plot::Curve;
        p.add();
    }
}


void OutputPlot::showPlotWidget() {
    Q_ASSERT(plotWidget);
    plotWidget->show();
}

} //namespace

