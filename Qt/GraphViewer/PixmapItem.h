#ifndef GRAPHICSITEM_H
#define GRAPHICSITEM_H

#include <QGraphicsPixmapItem>
#include <QGraphicsItem>

class PixmapItem :  public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit PixmapItem(QGraphicsItem *parent = 0);

    void setPixmap(const QPixmap &pixmap);

signals:

public slots:


};

#endif // GRAPHICSITEM_H
