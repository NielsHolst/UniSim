/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_SCREEN_EFFECTUATOR_H
#define VG_SCREEN_EFFECTUATOR_H


#include <QPair>
#include <QMap>
#include <usbase/model.h>
#include <usbase/string_map.h>

namespace vg {

class Screen : public UniSim::Model
{
public:
    Screen(UniSim::Identifier name, QObject *parent);

    enum Position {WholeRoof, FlatRoof, Roof1, Roof2, Side1, Side2, End1, End2};
    enum Layer {Inner, Mid, Outer};

    // Special methods
    Position pullPosition() const;
    Layer pullLayer() const;
    const double *state();

private:
    friend class Screens;

    // Parameters
    QString positionStr, layerStr;
    double lightTransmission, energyLossReduction, haze,
        airTransmission;
    // Data
    typedef UniSim::StringMap<Position> Positions;
    typedef UniSim::StringMap<Layer> Layers;
    static Positions positions;
    static Layers layers;
    const double *_state;
    // Methods
    void setStandardPositions();
    void setStandardLayers();
};
} //namespace


#endif
