#ifndef UNISIM_RECORDS_H
#define UNISIM_RECORDS_H

#include <QDate>
#include <QDateTime>
#include <QFile>
#include <QTime>
#include <QVector>
#include <usbase/model.h>

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
    void openFile();
    void readLineItems();
    void readColumnNames();
    void advanceFirstLine();
    void advanceLine();
    void extractValues();

    // parameters
    QString fileName;
    /*QDate startingDate;
    QTime startingTime;*/

    // pull variables
    QDate currentDate, nextDate;
    QTime currentTime, nextTime;
    QVector<double> values;

    // links
    UniSim::Model *calendar;

    // housekeeping
	QFile file;
    QStringList lineItems;
    QList<Identifier> columnNames;
    int dateColumn, timeColumn;
    QVector<double> *currentColumnValues, *nextColumnValues;

    /*
    QDateTime curTime, time[2];
	typedef QList<PullVariableBase*> Variables;
	typedef Variables* VariablesPtr;
	Variables curValues;
	VariablesPtr variables[2];
    */
};

}
#endif
