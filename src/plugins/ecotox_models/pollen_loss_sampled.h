/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef ECOTOX_POLLEN_LOSS_SAMPLED
#define ECOTOX_POLLEN_LOSS_SAMPLED
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>
#include <QObject>
#include <usbase/model.h>
#include <usbase/random.h>

namespace ecotox {

class PollenLossSampled : public UniSim::Model
{
	Q_OBJECT
public: 
    PollenLossSampled(UniSim::Identifier name, QObject *parent=0);
    ~PollenLossSampled();
	// standard methods
    void initialize();
	void reset();
    void update();

private:
    // parameters
    QString ratesAsString;

    // derived
    QList<double> rates;

    // pull variables
    double value;

    // random number generation
    typedef boost::uniform_int<int>  Distribution;
    typedef boost::variate_generator<UniSim::Random::Generator&, Distribution> Variate;
    Distribution *distribution;
    Variate *variate;
};

} //namespace
#endif
