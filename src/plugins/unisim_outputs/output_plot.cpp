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
#include <usbase/data_grid.h>
#include <usbase/exception.h>
#include <usbase/file_locations.h>
#include <usbase/model.h>
#include <usbase/named_object.h>
#include <usbase/object_pool.h>
#include <usbase/parameter.h>
#include <usbase/trace_base.h>
#include <usbase/utilities.h>
#include <usbase/main_window_interface.h>
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

void OutputPlot::amend() {
    Output::amend();
    if (yTraces().size() == 0) {
        QString msg = QString("Output plot '%1'' has no y-results").arg(id().label());
        throw Exception(msg, this);
    }
    if (xTraces().size() == 0) {
        QString msg = QString("Output plot '%1'' has no x-results").arg(id().label());
        throw Exception(msg, this);
    }
    else if (xTraces().size() > 1) {
        QString msg = QString("Output plot '%1'' has more than one x-result:").arg(id().label());
        for (int i = 0; i < xTraces().size(); ++ i)
            msg += "\n" + xTraces().at(i).label;
        throw Exception(msg, this);
    }

    QList<DataGrid*> tables = seekChildren<DataGrid*>("*");
    for (int i = 0; i < tables.size(); ++i) {
        TableRecord rec;
        rec.data = tables[i];
        rec.initX();
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

void OutputPlot::TableRecord::initX() {
    int nRows = data->rowNumber();
    QString xString = data->cell(0,0);
    try {
        stringToValue<QDate>(xString);
        Model *calendar = seekOne<Model*>("calendar");
        QDate initialDate = calendar->pullValue<QDate>("initialDate");
        QVector<QDate> xDate = data->column<QDate>(0);
        for (int i = 0; i < nRows; ++i) {
            x << initialDate.daysTo(xDate[i]);
        }
    }
    catch(Exception &) {
        try {
            stringToValue<QTime>(xString);
            Model *calendar = seekOne<Model*>("calendar");
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

void OutputPlot::createPlotWidget() {
    Q_ASSERT(mainWindow);
    plotWidget = mainWindow->createPlotWidget(title);
    plotWidget->showLegend(true);
}

void OutputPlot::fillPlotWidget() {
    TraceBase *x = xTraces()[0].trace;
    QString yAxisTitle(" ");
    plotWidget->setXYtitles(x->id().label(), yAxisTitle);
    for (int i = 0; i < yTraces().size(); ++i) {
        add(*x, yTraces()[i]);
    }

    for (int i = 0; i < _tableRecords.size(); ++i) {
        TableRecord &rec(_tableRecords[i]);
        for (int j = 0; j < rec.yList.size(); ++j) {
            add(&(rec.x), rec.yList[i]);
        }
    }

}

void OutputPlot::add(TraceBase &x, TraceRecord  &y) {
    Plot p;
    p.x = x.history();
    p.y = y.trace->history();
    p.yLegend = y.label;
    p.showLegend = (runNumber() == 1 || hasSummary());
    p.plotWidget = plotWidget;

    p.logy = logy;
    p.ymin = ymin;
    p.ymax = ymax;

    QColor color = colors[y.index % colors.size()];
    QPen pen = QPen(color);
    pen.setWidth(penWidth);
    p.pen = pen;
    p.symbol = new QwtSymbol(QwtSymbol::Ellipse, QBrush(), pen, QSize(symbolSize,symbolSize));
    p.type = y.trace->type();
    p.add();
}

void OutputPlot::add(QVector<double> *x, YRecord &y) {
    Plot p;
    p.x = x;
    p.y = &(y.data);
    p.yLegend = y.label;
    p.showLegend = true;
    p.plotWidget = plotWidget;

    p.logy = logy;
    p.ymin = ymin;
    p.ymax = ymax;

    QColor color = colors[y.index % colors.size()];
    QPen pen = QPen(color);
    pen.setWidth(penWidth);
    p.pen = pen;
    p.symbol = new QwtSymbol(QwtSymbol::Ellipse, QBrush(), pen, QSize(symbolSize,symbolSize));
    p.type = TraceBase::Symbols;
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

