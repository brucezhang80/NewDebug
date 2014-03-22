#include "ThumbnailItem.h"

#include <QEvent>
#include <QPainter>
#include <QGraphicsWidget>

#include <QDebug>

ThumbnailItem::ThumbnailItem(QGraphicsItem *parent, QGraphicsScene *scene) :
    QGraphicsPixmapItem(parent, scene),
    m_presented(false),m_filename(""),
    m_pressed(false)
{
    this->setAcceptHoverEvents(true);
    //this->setCursor(Qt::PointingHandCursor);
}

void ThumbnailItem::setNormalImage()
{
    QGraphicsPixmapItem::setPixmap(m_normalPix);
}

void ThumbnailItem::setHoverImage()
{
    QGraphicsPixmapItem::setPixmap(m_hoveredPix);
}

void ThumbnailItem::setPixmap(const QPixmap &normal, const QPixmap &hover)
{
    m_normalPix = normal;
    m_hoveredPix = hover;

    QGraphicsPixmapItem::setPixmap(m_presented ? m_hoveredPix : m_normalPix);
}

bool ThumbnailItem::sceneEvent(QEvent *event)
{
    if(event->type() == QEvent::GraphicsSceneHoverLeave && m_presented == false)
    {
        QGraphicsPixmapItem::setPixmap(m_normalPix);
    }
    else if(event->type() == QEvent::GraphicsSceneHoverEnter ||
            event->type() == QEvent::GraphicsSceneHoverMove)
    {
        QGraphicsPixmapItem::setPixmap(m_hoveredPix);
    }

    return QGraphicsPixmapItem::sceneEvent(event);
}

void ThumbnailItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit thumbnailClicked(m_filename);
    m_pressedPix = QPixmap( m_filename );

    QPixmap normalPixmap( 70, 100);
    normalPixmap.fill(Qt::transparent);
    QPainter paint(&normalPixmap);
    paint.setCompositionMode(QPainter::CompositionMode_Lighten);
    QPen pen(Qt::white);
    paint.setPen(pen);
    paint.fillRect(0, 10, 70 , 100, QColor::fromRgba(qRgba(255,255,255,200)));
    paint.drawPixmap(0, 10, 70, 100, m_normalPix);
    paint.end();

    QGraphicsPixmapItem::setPixmap(normalPixmap);
    m_pressed = true;
    QGraphicsPixmapItem::mousePressEvent(event);
}
