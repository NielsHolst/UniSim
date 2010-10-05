/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_WEATHER_FILE
#define UNISIM_WEATHER_FILE
#include <QFile>
#include <QList>
#include <QStringList>
#include <usbase/model.h>

namespace UniSim{

class WeatherFile : public UniSim::Model
{
	Q_OBJECT
public: 
    WeatherFile(UniSim::Identifier name, QObject *parent=0);
    ~WeatherFile();
    // standard methods
    virtual void initialize();
    virtual void reset();
    virtual void update();
    virtual void cleanup();

    // special methods
    void setColumn(QString name, int defaultColumn);

private:
    // parameters
    QString fileName;
    QDate firstDate;
    char separator;
    bool keepEmptyColumns;
    int headerLines;
    double timeStep;

    // file columns
    class Column {
        public:
        Column(){}
        Column(QString variableName, int defaultColumn, UniSim::Model *parent);
        void parseLine(const QStringList &items);
        private:
        QString variableName;
        int defaultColumn, column;
        double value;
    };
    typedef QList<Column*> Columns;
    Columns columns;
    QFile file;
    int lineNo;

    // state
    bool hasBeenReset;
    QString line;

    // methods
    bool readLine();
};

} //namespace
#endif
