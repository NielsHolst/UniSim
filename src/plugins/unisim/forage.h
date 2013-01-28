/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_FORAGE_H
#define UNISIM_FORAGE_H

#include <QVector>
#include <usbase/matrix.h>

namespace UniSim{
    namespace Forage{
    QVector<double> parts(const QVector<double> &proportionsWanted);
    QVector<int> firstLevel(int n);
    QVector<int> finalLevel(int from, int to);
    Matrix<double> owners(int n);
    Matrix<double> weights(int n);
    QVector<double> forage(const QVector<double> &proportionsWanted);
    }
}
#endif
