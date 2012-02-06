/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef AWE_ROTATION_H
#define AWE_ROTATION_H

#include <QList>
#include <QMap>
#include <QObject>
#include <QStringList>
#include <usbase/model.h>

using UniSim::Model;

namespace awe {

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
    QString cropsString;

    // state
    enum {NotSown, Growing, Harvested} state;
    int currentCropIndex;
    double lai;

    // decoded parameters
    QStringList cropNames;

    // links
    Model *_calendar;
    QList<Model*> crops;

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
