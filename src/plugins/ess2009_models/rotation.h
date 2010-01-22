/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef ESS2009_ROTATION_H
#define ESS2009_ROTATION_H

#include <QList>
#include <QMap>
#include <QObject>
#include <QStringList>
#include <usbase/model.h>

using UniSim::Model;

namespace ess2009 {

class Crop;

class Rotation : public Model
{
    Q_OBJECT
public:
    Rotation(UniSim::Identifier name, QObject *parent=0);

    void initialize();
    void reset();
    void update();

    Model* currentCrop();
    QList<Model*> cropRotation();   // Pointer to crop model for each year in rotation
    QList<Model*> cropModels();     // Pointer to each crop model owned by rotation

private:
    // parameters
    QString _cropsString;

    // state
    enum {NotSown, Growing, Harvested} _state;
    int _currentCropIndex;
    double _lai;

    // decoded parameters
    QStringList _cropNames;

    // links
    Model *_calendar;
    QList<Model*> _crops;

    // methods
    void collectRotation();
    void connectCrops();
    void decodeCrops();
    void chopParentheses(QString &s) const;

private slots:
    void handleEvent(QObject *sender, QString eventName);
};

}
#endif
