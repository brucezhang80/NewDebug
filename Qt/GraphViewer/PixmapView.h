#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>

class PixmapItem;

class PixmapView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit PixmapView(QWidget *parent = 0);
    void setImage(const QPixmap &pixmap);
    void reset();

signals:

public slots:

protected:
    void resizeEvent(QResizeEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    QGraphicsScene*     m_sceneBig;
    PixmapItem*         m_itemBig;
    qreal               m_scaleSize;
};

#endif // GRAPHICSVIEW_H
