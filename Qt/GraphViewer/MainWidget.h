#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QLineEdit>

class PixmapView;
//class ThumbnailView;
class ThumbnailBar;

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

private slots:
    void imageChanged(const QString &filename);
    void loadImages();

protected:
    void resizeEvent(QResizeEvent *);

private:
    Ui::MainWidget *ui;
    QLineEdit*      m_editPath;

    PixmapView*     m_pixmaphView; // is ui->graphicsView;
    ThumbnailBar*   m_thumbnailBar;
};

#endif // MAINWIDGET_H
