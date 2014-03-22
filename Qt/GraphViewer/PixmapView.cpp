#include "PixmapView.h"

#include <PixmapItem.h>
#include <QWheelEvent>
#include <QDebug>

PixmapView::PixmapView(QWidget *parent) :
    QGraphicsView(parent),
    m_scaleSize(0)
{
    this->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);
    this->setMouseTracking(true);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
    this->setDragMode(QGraphicsView::ScrollHandDrag);
    this->setBackgroundBrush( QBrush(QColor(103,103,103,255)) );//QImage("I:/Yvi/images/background.jpg")
    this->setCacheMode(QGraphicsView::CacheBackground);

    m_sceneBig = new QGraphicsScene;
    this->setScene(m_sceneBig);
    m_sceneBig->setSceneRect(this->rect());
    m_itemBig = new PixmapItem;
    m_sceneBig->addItem(m_itemBig);
}

void PixmapView::setImage(const QPixmap &pixmap)
{
    m_sceneBig->clear();
    PixmapItem* itemBig = new PixmapItem;
    m_sceneBig->addItem(itemBig);
    itemBig->setPixmap(pixmap);
    QRectF rectf = m_sceneBig->itemsBoundingRect();
    m_sceneBig->setSceneRect( rectf );

    if( this->viewport()->width() < m_sceneBig->width() ||
            this->viewport()->height() < m_sceneBig->height() )
    {
        fitInView(itemBig, Qt::KeepAspectRatio);
    }
    this->viewport()->update();
}

void PixmapView::reset()
{
    m_sceneBig->clear();
}

void PixmapView::resizeEvent(QResizeEvent *event)
{
    QRectF rectf = m_sceneBig->itemsBoundingRect();
    m_sceneBig->setSceneRect( rectf );
//    this->fitInView(m_sceneBig->sceneRect());
    if( this->viewport()->width() < m_sceneBig->width() ||
            this->viewport()->height() < m_sceneBig->height() )
    {
        fitInView(m_sceneBig->items().at(0), Qt::KeepAspectRatio);
    }
    this->viewport()->update();

    QGraphicsView::resizeEvent(event);
}

void PixmapView::wheelEvent(QWheelEvent *event)
{
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(AnchorViewCenter);

    qreal scaleFactor = 1.2;//event->delta() / 120;
    if(event->delta() > 0)
    {

    }
    else
    {
        scaleFactor = 1.0 / scaleFactor;
    }
    m_itemBig->scale(scaleFactor, scaleFactor);
    scaleFactor *= scaleFactor;
    QRectF rectf = m_sceneBig->itemsBoundingRect();
    m_sceneBig->setSceneRect(rectf.x(), rectf.y(), m_itemBig->boundingRect().width(),
                                m_itemBig->boundingRect().height());
}
