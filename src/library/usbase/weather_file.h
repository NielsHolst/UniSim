/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_WEATHER_FILE
#define UNISIM_WEATHER_FILE
#include <QFile>
#include <QMap>
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
    bool variableExists(QString name);

private:
    // parameters
    QString fileName;
    QDate firstDate;

    // file columns
    class Column {
        public:
        Column(){}
        Column(QString variableName, int defaultColumn, UniSim::Model *parent);
        void setParameter(UniSim::Model *parent);
        void updateState(const QStringList &items);
        private:
        QString variableName;
        int defaultColumn, column;
        double value;
    };
    typedef QMap<QString, Column*> Columns;
    Columns columns;
    QFile file;
    int lineNo;

    // state
    bool hasBeenReset;

    // methods
    void updateRecordings();
    void updatePar(double hour);

};

} //namespace
#endif
