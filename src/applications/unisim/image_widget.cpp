/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include <usbase/exception.h>
#include "image_widget.h"

namespace UniSim {

ImageWidget::ImageWidget()
  : wasPainted(false)
{
    pixmap = new QPixmap;
}

ImageWidget::~ImageWidget() {
    delete pixmap;
}

QSize ImageWidget::sizeHint () const {
    return pixmap->size();
}

void ImageWidget::setImage(QString aFilePath) {
  bool ok = pixmap->load(aFilePath);
  if (!ok || pixmap->isNull()) throw Exception("Could not show image in file: " + aFilePath);
  update();
}

void ImageWidget::paintEvent(QPaintEvent *)
{
  QPainter painter(this);
  if (wasPainted)
    painter.drawPixmap(0, 0, pixmap->scaled(size()));
  else {
    painter.drawPixmap(0, 0, *pixmap);
    wasPainted = true;
  }
}

}
