/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_IMAGE_WIDGET_H
#define UNISIM_IMAGE_WIDGET_H

#include <QSize>
#include <QWidget>

class QPaintEvent;
class QPixmap;

namespace UniSim {

class ImageWidget : public QWidget
{
public:
    ImageWidget();
    ~ImageWidget();
    QSize sizeHint () const;
    void setImage(QString aFilePath);
protected:
    void paintEvent(QPaintEvent *);
private:
    QPixmap *pixmap;
    bool wasPainted;
};

}
#endif
