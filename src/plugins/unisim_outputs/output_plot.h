/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_OUTPUT_PLOT
#define UNISIM_OUTPUT_PLOT

#include <QColor>
#include <QList>
#include <usbase/output.h>

namespace UniSim{

class MainWindowInterface;
class PlotWidget;

class OutputPlot : public Output
{
	Q_OBJECT
public:
    OutputPlot(Identifier name, QObject *parent=0);
    ~OutputPlot();
    // standard methods
	void initialize();
    void cleanup();
    void debrief();
	
private:
    // parameters
    QString title;

    // housekeeping
    MainWindowInterface *mainWindow;
    PlotWidget *plotWidget;
    static QList<QColor> colors;
    QList<Identifier> yLabels;

    // methods
    void showPlot();
    void createPlotWidget();
    void fillPlotWidget();
    void fillWithResults();
    void fillWithData();
    void showPlotWidget();
    bool emptyResults() const;
    bool emptyData() const;
    void setYLabels();
    bool setYLabelsFromLabels();
    void setYLabelsFromIds();
    QList<Identifier> getIds(QList<NamedObject*> &objects, bool *areEqual) const;
};

} //namespace


#endif
