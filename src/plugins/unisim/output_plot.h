/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_OUTPUT_OUTPUT_PLOT
#define UNISIM_OUTPUT_OUTPUT_PLOT

#include <QColor>
#include <QList>
#include <QVector>
#include <qwt_symbol.h>
#include <usbase/output.h>

namespace UniSim{

class DataGrid;
class MainWindowInterface;
class PlotWidget;
class Trace;

class OutputPlot : public Output
{
	Q_OBJECT
public:
    OutputPlot(Identifier name, QObject *parent=0);
    ~OutputPlot();
    // standard methods
    void amend();
    void cleanup();
    void debrief();

    struct YRecord {
        QVector<double> data;
        QString label;
        int index;
    };

    struct TableRecord {
        DataGrid *data;
        QVector<double> x;
        QList<YRecord> yList;
        void initX();
        void initY();
    };
    // methods for unit tests
    const QList<TableRecord>& tableRecords();

private:
    // parameters
    QString title;
    bool logy;
    double ymin, ymax;
    int penWidth, symbolSize;

    // housekeeping
    static QList<QColor> colors;
    static QList<QwtSymbol> symbols;
    MainWindowInterface *mainWindow;
    PlotWidget *plotWidget;

    QList<TableRecord> _tableRecords;

    // methods
    void createDesignInfo();
    void showPlot();
    void createPlotWidget();
    void fillPlotWidget();
    void add(Trace &x, TraceRecord  &y);
    void add(QVector<double> *x, YRecord &y);
    void showPlotWidget();
    void setZoomer();
};

} //namespace


#endif
