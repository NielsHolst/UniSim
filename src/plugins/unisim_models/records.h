#ifndef UNISIM_RECORDS_H
#define UNISIM_RECORDS_H

#include <QDate>
#include <QDateTime>
#include <QFile>
#include <QTime>
#include <QVector>
#include <usbase/model.h>
#include <usbase/file_location_info.h>

namespace UniSim {

class PullVariableBase;

class Records : public Model
{
    Q_OBJECT
public:
    Records(UniSim::Identifier name, QObject *parent=0);
    ~Records();
    // standard methods
    void initialize();
    void reset();
    void update();
    void cleanup();

private:
    // methods
    void setFileLocationType();
    void openFile();
    void readLineItems();
    void readColumnNames();
    void createColumnPullVariables();
    void readFromFirstToLastLine();
    void readToInitialYear();
    void readToFirstLine();
    void advanceFirstLine();
    void advanceLine();
    void extractValues();
    void advanceTime();

    // parameters
    QString fileName, fileLocation;
    bool randomizeInitialYear, imposeInitialDateTime;
    FileLocationInfo::FileType fileLocationType;

    // pull variables
    QDateTime currentDateTime, nextDateTime, firstDateTime, lastDateTime;
    QDate currentDate, nextDate;
    QTime currentTime, nextTime;
    QVector<double> values;

    // links
    UniSim::Model *calendar;

    // housekeeping
	QFile file;
    QStringList lineItems;
    bool pastLastLine;
    QList<Identifier> columnNames;
    int dateColumn, timeColumn;
    QVector<double> *currentColumnValues, *nextColumnValues;
};

}
#endif
