/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INTERCOM_TIME_SLICE
#define INTERCOM_TIME_SLICE
#include <QObject>
#include "photosynthetic_rate.h"

namespace UniSim {
    class Model;
}

namespace intercom{

class Plant;

class TimeSlice : public QObject
{
	Q_OBJECT
public: 
    TimeSlice(int slice, QObject *parent);
    PhotosyntheticRate calcPhotosynthesis();
private:
    QList<Plant*> plants;
    UniSim::Model *calendar;
    int slice;

    void lookup();
};

} //namespace
#endif
