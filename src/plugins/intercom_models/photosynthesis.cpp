/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <usbase/clock.h>
#include <usbase/model.h>
#include <usbase/utilities.h>
#include "constants.h"
#include "photosynthesis.h"
#include "time_slice.h"

using namespace UniSim;

namespace intercom{

Photosynthesis::Photosynthesis(QObject *parent)
    : QObject(parent)
{
}

void Photosynthesis::initialize() {
    for (int i = 0; i < 3; ++i) {
        QString name = "slice" + QString::number(i);
        TimeSlice *slice = new TimeSlice(name, this, i);
        timeSlices.append(slice);
        slice->initialize();
    }
}

PhotosyntheticRate Photosynthesis::calculate() {
    PhotosyntheticRate result;
    for (int i = 0; i < timeSlices.size(); ++i) {
        PhotosyntheticRate a = timeSlices[i]->calcPhotosynthesis();
        result += a;
        cout << " Photosynthesis::calculate()\n"
                << "a      = " << a.absorption() << " " << a.assimilation() << "\n"
                << "result = " << result.absorption() << " " << result.assimilation() << "\n";
    }
    return result;
}

} //namespace

