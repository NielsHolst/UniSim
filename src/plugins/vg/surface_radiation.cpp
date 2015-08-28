/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <qglobal.h>
#include <QMessageBox>
#include <QString>
#include <usbase/test_num.h>
#include "general.h"
#include "surface_radiation.h"

namespace vg {

SurfaceRadiation::SurfaceRadiation() {
    asScreen(1,0,0);
}

SurfaceRadiation& SurfaceRadiation::asCover(double transmissivity, double absorptivity, double emissivity) {
    light.tra = transmissivity;
    ir.tra = 0.;
    light.outer.abs =
    light.inner.abs = absorptivity;
    ir.outer.abs =
    ir.inner.abs = emissivity;
    light.outer.setRef(light.tra);
    light.inner.setRef(light.tra);
    ir.outer.setRef(ir.tra);
    ir.inner.setRef(ir.tra);
    return *this;
}

SurfaceRadiation& SurfaceRadiation::asScreen(double transmissivity, double absorptivityIrOuter, double absorptivityIrInner) {
    light.tra = transmissivity;
    ir.tra = 0.;
    light.outer.abs =
    ir.outer.abs = absorptivityIrOuter;
    light.outer.setRef(light.tra);
    ir.outer.setRef(ir.tra);

    ir.inner.abs = absorptivityIrInner;
    ir.inner.setRef(ir.tra);
    light.inner.ref = std::min(ir.inner.ref, 1. - light.tra);
    light.inner.setAbs(light.tra);
    return *this;
}

void SurfaceRadiation::Spectrum::Direction::setRef(double tra) {
    ref = 1. - abs - tra;
    TestNum::snapToZero(ref);
    if (ref < 0.) {
        QMessageBox::information(0, "Test ref<0", QString::number(ref));
    }
    Q_ASSERT(ref>=0);
}

void SurfaceRadiation::Spectrum::Direction::setAbs(double tra) {
    abs = 1. - ref - tra;
    TestNum::snapToZero(abs);
    if (abs < 0.) {
        QMessageBox::information(0, "Test abs<0", QString::number(abs) + " " + QString::number(ref) + " " + QString::number(tra));
    }
    Q_ASSERT(abs>=0.);
}

SurfaceRadiation::Spectrum& SurfaceRadiation::Spectrum::operator*=(const SurfaceRadiation::Spectrum &s2) {
    double k = 1. - inner.ref*s2.outer.ref,
           t12 = tra*s2.tra/k,
           r12_outer = outer.ref + sqr(tra)*s2.outer.ref,
           a12_outer = 1. - r12_outer - t12,

           r12_inner = s2.inner.ref + sqr(s2.tra)*inner.ref,
           a12_inner = 1. - r12_inner - t12;
    tra = t12;
    inner.abs = a12_inner;
    inner.ref = r12_inner;
    outer.abs = a12_outer;
    outer.ref = r12_outer;
    return *this;
}

SurfaceRadiation& SurfaceRadiation::operator*=(const SurfaceRadiation &s2) {
    light *= s2.light;
    ir *= s2.ir;
    return *this;
}

} //namespace

