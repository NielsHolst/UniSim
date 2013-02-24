/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_COLOURS
#define UNISIM_COLOURS
#include <QColor>
#include <QVector>

namespace UniSim{

class Colours
{
public: 
    Colours();
    static const QVector<QColor>& standard();
    static QString toHex(QColor color);
private:
    static QVector<QColor> _standard;

};


} //namespace
#endif
