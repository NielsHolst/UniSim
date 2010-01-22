/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INTERCOM_ABSORPTION_EXPONENTS
#define INTERCOM_ABSORPTION_EXPONENTS

#include <QVector>

namespace intercom{

class AbsorptionExponents
{
public: 
    typedef enum {Diffuse, Direct, DirectTotal, NumLightComponents} LightComponent;

    AbsorptionExponents();
    void reset();
    void accumulate(const AbsorptionExponents &increments);
    double value(int lc) const;
    double value(LightComponent lc) const;

    double& operator[] (int lc);
    double& operator[] (LightComponent lc);
private:
    QVector<double> exponents;
};

} //namespace
#endif
