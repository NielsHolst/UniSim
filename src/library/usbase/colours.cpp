/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "colours.h"

namespace UniSim{

QVector<QColor> Colours::_standard;

Colours::Colours()
{
}

const QVector<QColor>& Colours::standard() {
    if (_standard.isEmpty()) {
        _standard
        << QColor("#FF416E")
        << QColor("#3760D5")
        << QColor("#FFC741")
        << QColor("#34C1E8")
        << QColor("#F7FF41")
        << QColor("#A135D0")
        << QColor("#39DE52");
    }
    return _standard;
}

QString Colours::toHex(QColor color) {
    int r, g, b;
    color.getRgb(&r, &g, &b);
    return "#"
            + QString::number(r,16)
            + QString::number(g,16)
            + QString::number(b,16);

}

} //namespace
