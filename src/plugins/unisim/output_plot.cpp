/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
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
#include <qwt_symbol.h>
#include <usbase/data_grid.h>
#include <usbase/exception.h>
#include <usbase/file_locations.h>
#include <usbase/model.h>
#include <usbase/named_object.h>
#include <usbase/object_pool.h>
#include <usbase/parameter.h>
#include <usbase/trace.h>
#include <usbase/utilities.h>
#include <usbase/main_window_interface.h>
#include <usengine/plot_widget.h>
#include "plot.h"
#include "output_plot.h"
#include "publish.h"

namespace UniSim{

PUBLISH2(OutputPlot, Plot)

QList<QColor> OutputPlot::colors;

OutputPlot::OutputPlot(Identifier name, QObject *parent)
    : OutputBase(name, parent), plotWidget(0)
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
}

OutputPlot::~OutputPlot() {
    delete plotWidget;
}

void OutputPlot::amend() {
    OutputBase::amend();
    if (traceRecords().size() < 2) {
        QString msg("Output plot titled '%1' must have at least two traces");
        throw Exception(msg.arg(title), this);
    }

    QList<DataGrid*> tables = seekChildren<DataGrid*>("*");
    for (int i = 0; i < tables.size(); ++i) {
        TableRecord rec;
        rec.data = tables[i];
        rec.initX(this);
        rec.initY();
        _tableRecords << rec;
    }

    try {
        mainWindow = objectPool()->find<MainWindowInterface*>("mainWindow");
    }
    catch (Exception &) {
        mainWindow = 0;
    }
}

void OutputPlot::TableRecord::initX(OutputPlot *parent) {
    int nRows = data->rowNumber();
    QString xString = data->cell(0,0);
    try {
        stringToValue<QDate>(xString);
        Model *calendar = parent->seekOne<Model*>("calendar");
        QDate initialDate = calendar->pullValue<QDate>("initialDate");
        QVector<QDate> xDate = data->column<QDate>(0);
        for (int i = 0; i < nRows; ++i) {
            x << initialDate.daysTo(xDate[i]);
        }
    }
    catch(Exception &) {
        try {
            stringToValue<QTime>(xString);
            Model *calendar = parent->seekOne<Model*>("calendar");
            QTime initialTime = calendar->pullValue<QTime>("initialTimeOfDay");
            QVector<QTime> xTime = data->column<QTime>(0);
            for (int i = 0; i < nRows; ++i) {
                x << initialTime.secsTo(xTime[i]);
            }
        }
        catch(Exception &) {
            x = data->column<double>(0);
        }
    }
}

void OutputPlot::TableRecord::initY() {
    for (int i = 1; i < data->columnNumber(); ++i) {
        YRecord rec;
        rec.data = data->column<double>(i);
        rec.label = data->columnNames()[i];
        rec.index = i-1;
        yList << rec;
    }
}

void OutputPlot::cleanup() {
    if (mainWindow && !hasSummary()) {
        if (runNumber() == 1) createPlotWidget();
        showPlot();
        setZoomer();
    }
}

void OutputPlot::debrief() {
    if (mainWindow && hasSummary()) {
        createPlotWidget();
        showPlotWidget();
        showPlot();
        setZoomer();
    }
}

int OutputPlot::numXTraces() const {
    return 1;
}

void OutputPlot::showPlot() {
    fillPlotWidget();
    showPlotWidget();
    mainWindow->tile();
}

void OutputPlot::setZoomer() {
    QwtPlotCanvas *canvas = dynamic_cast<QwtPlotCanvas*>( plotWidget->plot()->canvas() );
    Q_ASSERT(canvas);
    QwtPlotZoomer* zoomer = new QwtPlotZoomer( canvas );
    zoomer->setRubberBandPen( QColor( Qt::black ) );
    zoomer->setTrackerPen( QColor( Qt::black ) );
    // Walk up zoom stack
    zoomer->setMousePattern( QwtEventPattern::MouseSelect3, Qt::RightButton );
    // Walk down zoom stack
    zoomer->setMousePattern( QwtEventPattern::MouseSelect6, Qt::RightButton, Qt::ShiftModifier );
}

void OutputPlot::createPlotWidget() {
    Q_ASSERT(mainWindow);
    plotWidget = mainWindow->createPlotWidget(title);
    Q_ASSERT(plotWidget);
    plotWidget->showLegend(true);
}

void OutputPlot::fillPlotWidget() {
    Trace *x = traceRecords()[0].trace;
    QString yAxisTitle(" ");
    plotWidget->setXYtitles(x->id().label(), yAxisTitle);
    for (int i = 1; i < traceRecords().size(); ++i) {
        add(*x, traceRecords()[i], i-1);
    }

    for (int i = 0; i < _tableRecords.size(); ++i) {
        TableRecord &rec(_tableRecords[i]);
        for (int j = 0; j < rec.yList.size(); ++j) {
            add(&(rec.x), rec.yList[i], i);
        }
    }

}

void OutputPlot::add(Trace &x, TraceRecord  &y, int index) {
    Plot p;
    p.x = x.history();
    p.y = y.trace->history();
    p.yLegend = y.label;
    p.showLegend = (runNumber() == 1 || hasSummary());
    p.plotWidget = plotWidget;

    p.logy = logy;
    p.ymin = ymin;
    p.ymax = ymax;

    QColor color = colors[index % colors.size()];
    QPen pen = QPen(color);
    pen.setWidth(penWidth);
    p.pen = pen;
    p.symbol = new QwtSymbol(QwtSymbol::Ellipse, QBrush(), pen, QSize(symbolSize,symbolSize));
    p.type = y.trace->type();
    p.add();
}

void OutputPlot::add(QVector<double> *x, YRecord &y, int index) {
    Plot p;
    p.x = x;
    p.y = &(y.data);
    p.yLegend = y.label;
    p.showLegend = true;
    p.plotWidget = plotWidget;

    p.logy = logy;
    p.ymin = ymin;
    p.ymax = ymax;

    QColor color = colors[index % colors.size()];
    QPen pen = QPen(color);
    pen.setWidth(penWidth);
    p.pen = pen;
    p.symbol = new QwtSymbol(QwtSymbol::Ellipse, QBrush(), pen, QSize(symbolSize,symbolSize));
    p.type = Trace::Symbols;
    p.add();
}

void OutputPlot::showPlotWidget() {
    Q_ASSERT(plotWidget);
    plotWidget->show();
}

const QList<OutputPlot::TableRecord>& OutputPlot::tableRecords() {
    return _tableRecords;
}

} //namespace

