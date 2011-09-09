/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_OUTPUT_DATA_H
#define UNISIM_OUTPUT_Data_H

#include <QList>
#include <QVector>
#include "component.h"

namespace UniSim{	

class Dataset;
class PullVariableBase;

class OutputData : public Component
{
	Q_OBJECT
public:
    OutputData(QString label, QString axis, Dataset *dataset, QString columnName, QObject *parent);

    // special methods
    typedef enum {XAxis, YAxis} Axis;
    Axis axis() const;
    QVector<double>* data() const;

private:
    // methods
    void setAxisFromString(QString axis);

    // links
    Dataset *dataset;		        //!< Pointer to dataset holding the data column

    // data
    Axis _axis;
    QString columnName;

};

typedef QList<OutputData*> OutputDatas;

} //namespace


#endif
