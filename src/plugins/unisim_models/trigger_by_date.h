/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_TRIGGER_BY_DATE
#define UNISIM_TRIGGER_BY_DATE
#include <Qdate>
#include <QObject>
#include <usbase/model.h>

namespace UniSim{

class TriggerByDate : public Model
{
	Q_OBJECT
public: 
    TriggerByDate(Identifier name, QObject *parent=0);
    // standard methods
    void initialize();
    void reset();
    void update();
private:
    // methods
    void decodeFrequency();
    // parameters
    typedef enum {Daily, Monthly, Yearly, Once} Frequency;
    Frequency frequency;
    QString frequencyAsString;
    QDate byDate;
    bool triggerAtReset;
    // pull variables
    bool triggered;
    // links
    Model *calendar;
};

} //namespace
#endif
