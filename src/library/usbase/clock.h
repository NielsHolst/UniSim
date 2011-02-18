/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_CLOCK_H
#define UNISIM_CLOCK_H

#include <QObject>
#include <usbase/object_pool.h>

namespace UniSim{

class Clock : public QObject
{
    Q_OBJECT

public:
    Clock();
    static QString id();
    void doTick(double hour);

signals:
    void tick(double hour);

private:
    // Singleton
    static Clock* _clock;
    friend Clock* clock();
};

inline Clock* clock()
{
    if (!Clock::_clock)
        Clock::_clock = objectPool()->find<Clock*>(Clock::id());
    return Clock::_clock;
}

} //namespace

#endif

