#ifndef THUMBNAILVIEW_H
#define THUMBNAILVIEW_H

#include <QGraphicsView>
#include <QFileInfoList>

class ThumbnailItem;
class QFileInfo;

class ThumbnailView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ThumbnailView(QWidget *parent = 0);
    void loadCurrentDir();
    void setCurrentFile(const QString &filename) { m_currentFile = filename;}
    void setCurrentDir(const QString &dir) { m_currentDir = dir;}
    QString currentFile() const { return m_currentFile;}
    QString currentDir() const { return m_currentDir;}
    void reset();

signals:
    void thumbnailChanged(const QString &newImage);

public slots:

private:
    bool compareNamesFileInfo(const QFileInfo& fileinfo1,const QFileInfo& fileinfo2);
    void toNormalThumbnail(int index, QPixmap &normal, int width, int height,
                        int spacing, int liftUp);
    void toHoverThumbnail(int index, QPixmap &hover, int width, int height,
                        int spacing, int liftUp);

private:
    // The scene
    QGraphicsScene*         m_thumbScene;
    QList<ThumbnailItem*>   m_thumbItemList;

    // Some lists containing info about items in directory
    QFileInfoList           m_imagesInfo;
    QStringList             m_imagesPath;
    QString                 m_currentFile;
    QString                 m_currentDir;
};

#endif // THUMBNAILVIEW_H
