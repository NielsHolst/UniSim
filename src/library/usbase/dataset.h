/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_DATASET_H
#define UNISIM_DATASET_H
#include <QMap>
#include <QVector>
#include "component.h"

namespace UniSim{

class Dataset : public Component
{
	Q_OBJECT
public: 
    Dataset(Identifier name, QObject *parent=0);
    // standard methods
    void initialize();

    // special methods
    bool contains(QString columnName) const;
    QVector<double>* columnData(QString columnName);

private:
    // parameters
    QString fileName;
    QDate firstDate;
    char separator;
    bool keepEmptyColumns;
    int headerLines;

    QString columns;

    // pull variables
    int size;

    // state
    QFile file;
    struct ColumnData {
        int colIndex;
        QVector<double> values;
    };
    QMap< QString, ColumnData > data;

    // methods
    void initializeData();
    void itemToData(QStringList items, int colIndex, QString columnName);
};

} //namespace
#endif
