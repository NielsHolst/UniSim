/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef ANONYMOUS_MODEL_H
#define ANONYMOUS_MODEL_H

#include <QObject>
#include <QMap>
#include <usbase/model.h>

namespace UniSim{

//! \addtogroup UniSim
//@{
//! An %AnonymousModel simply acts as a container (parent) of other models
class AnonymousModel : public Model
{
	//Q_OBJECT
public:
//! Constructor
    /*!
      \param name of this object.
      \param parent of this object.
      \return This object.
    */
        AnonymousModel(Identifier name, QObject *parent=0);
};
//@}
} //namespace


#endif
