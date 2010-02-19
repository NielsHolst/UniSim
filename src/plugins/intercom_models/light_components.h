/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INTERCOM_LIGHT_COMPONENTS
#define INTERCOM_LIGHT_COMPONENTS

namespace intercom{

typedef enum {Diffuse, DirectDirect, DirectTotal} LightComponent;

class LightComponents
{
public: 
    LightComponents();
    void reset();
    void accumulate(const LightComponents &increment);
    double value(int lc) const;
    double value(LightComponent lc) const;

    double& operator[] (int lc);
    double& operator[] (LightComponent lc);
private:
    double components[3];
};

} //namespace
#endif
