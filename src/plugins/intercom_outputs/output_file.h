/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INTERCOM_OUTPUT_FILE
#define INTERCOM_OUTPUT_FILE

#include <QFile>
#include <QList>
#include <QTextStream>
#include <usbase/output.h>

namespace UniSim {
    class Model;
}

namespace intercom{

class Area;
class Layer;
class Plant;
class PlantLayers;
class TimeSlice;

class OutputFile : public UniSim::Output
{
	Q_OBJECT
public:
    OutputFile(UniSim::Identifier name, QObject *parent=0);
    // standard methods
    void initialize();
    void cleanup();
	
private:
    // parameters
    QString fileName;

    // housekeeping
    QFile file;
    QTextStream os;

    // methods
    void openFile();
    void seekObjects();

    Plant* plant(TimeSlice *slice);
    PlantLayers* plantLayers(TimeSlice *slice);
    QList<Layer*> layers(TimeSlice *slice);

    void writeWeather(TimeSlice *slice);
    void writeLayerHeights(TimeSlice *slice);
    void writeLayerELAI(TimeSlice *slice);
    void writeAbsorption(TimeSlice *slice);
    void writeAbsorption(TimeSlice *slice, Area *area);

    // links
    UniSim::Model *calendar, *weather;
    QList<TimeSlice*> slices;
};

} //namespace


#endif
