/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef ECOTOX_DEPOSITION_FLUSH
#define ECOTOX_DEPOSITION_FLUSH
#include <QObject>
#include <usbase/model.h>

namespace ecotox {

class PollenOnsetDate;

class DepositionFlush : public UniSim::Model
{
	Q_OBJECT
public: 
    DepositionFlush(UniSim::Identifier name, QObject *parent=0);
    ~DepositionFlush();
	// standard methods
    void amend();
    void initialize();
	void reset();
	void update();

private:
    // parameters
    double durationAsDouble;
    int duration;
    QString onsetFileName;

    // derived
    double scaling;
    QDate onsetDate;

    // pull variables
    double value, total, percentile;

    // links
    Model *calendar, *loss;
    PollenOnsetDate *pollenOnsetDate;

    // state
    enum {Before, Inside, After} phase;
    int phaseTime;

    // methods
    void calcScaling();
    inline double f(int x);

};

double DepositionFlush::f(int x) {
    return x*(duration-x);
}


} //namespace
#endif
