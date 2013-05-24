/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_FIXED_H
#define UNISIM_FIXED_H

#include <QDate>
#include <QTime>
#include <QVector>
#include <usbase/model.h>
#
namespace UniSim {

class Fixed : public Model
{
    //Q_OBJECT
public:
    Fixed(UniSim::Identifier name, QObject *parent=0);
    // standard methods
    void amend();
private:
    // parameters
    QString parametersAsString;

    // value buffers
    QVector<QDate> dates;
    QVector<QTime> times;
    QVector<bool> bools;
    QVector<int> ints;
    QVector<double> doubles;
    QVector<QString> strings;
};

}
#endif
