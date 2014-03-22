#ifndef GRAPHVIEWER_H
#define GRAPHVIEWER_H

#include <QMainWindow>
#include <QLineEdit>

class PixmapView;
class ThumbnailBar;

namespace Ui {
class GraphViewer;
}

class GraphViewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit GraphViewer(QWidget *parent = 0);
    ~GraphViewer();

private slots:
    void imageChanged(const QString &filename);
    void loadImages();

protected:
    void resizeEvent(QResizeEvent *);
    void closeEvent(QCloseEvent *);

private:
    Ui::GraphViewer *ui;
    QLineEdit*      m_editPath;

    PixmapView*     m_pixmaphView; // is ui->graphicsView;
    ThumbnailBar*   m_thumbnailBar;
};

#endif // GRAPHVIEWER_H
