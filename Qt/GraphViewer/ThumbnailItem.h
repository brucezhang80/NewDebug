#ifndef THUMBNAILITEM_H
#define THUMBNAILITEM_H

#include <QGraphicsPixmapItem>

class ThumbnailItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit ThumbnailItem(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    void    setNormalImage();
    void    setHoverImage();
    void    setThumbnailFilename(const QString &filename){ m_filename = filename;}
    QString thumbnailFilename() const { return m_filename; }

signals:
    void thumbnailClicked(const QString &filename);

public slots:
    void setPixmap(const QPixmap &normal, const QPixmap &hover);


protected:
    bool sceneEvent(QEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    // The normal and hovered pixmap
    QPixmap     m_normalPix;
    QPixmap     m_hoveredPix;
    QPixmap     m_pressedPix;

    bool        m_presented;
    bool        m_pressed;
    QString     m_filename;
};

#endif // THUMBNAILITEM_H
