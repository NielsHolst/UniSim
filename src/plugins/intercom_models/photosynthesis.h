/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INTERCOM_PHOTOSYNTHESIS
#define INTERCOM_PHOTOSYNTHESIS
#include <QObject>
#include "photosynthetic_rate.h"

namespace UniSim {
    class Model;
}

namespace intercom{

class TimeSlice;

class Photosynthesis : public QObject
{
	Q_OBJECT
public: 
    Photosynthesis(QObject *parent);
    PhotosyntheticRate calculate();

private:
    QList<TimeSlice*> timeSlices;
    UniSim::Model *calendar;
};

} //namespace
#endif
