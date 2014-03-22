#ifndef THUMBNAIL_H
#define THUMBNAIL_H

#include <QWidget>

class ThumbnailView;

class ThumbnailBar : public QWidget
{
    Q_OBJECT
public:
    explicit ThumbnailBar(QWidget *parent = 0);

    void hide();
    void show();
    void reset();

signals:
    void thumbnailChanged(const QString &);

public slots:
    void loadImages(const QString &);

protected:
    bool event(QEvent *event);
    void wheelEvent(QWheelEvent *);

private:
    ThumbnailView*  m_thumbView;
};

#endif // THUMBNAIL_H
