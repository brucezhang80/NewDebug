#include "PixmapItem.h"

#include <QPainter>
#include <QPixmap>
#include <QGraphicsScene>
#include <QGraphicsSceneWheelEvent>

#include <QDebug>

PixmapItem::PixmapItem(QGraphicsItem *parent) :
    QGraphicsPixmapItem(parent)
{
    setCacheMode(NoCache);
}

void PixmapItem::setPixmap(const QPixmap &pixmap)
{
//    QPixmap thisPixmap = this->pixmap();

    QPixmap pix(pixmap);
//    if( !pix.isNull() )
//    {
//        pix = QPixmap(pixmap.width(), pixmap.height());
//        pix.fill(QColor(0,0,0,1));
//        QPainter p(&pix);
//        p.drawPixmap(0, 0,pixmap.width(),pixmap.height(), pixmap);
//        p.end();
//    }

    QGraphicsPixmapItem::setPixmap(pix);
}
