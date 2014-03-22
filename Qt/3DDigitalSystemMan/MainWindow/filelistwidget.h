#ifndef FILELISTWIDGET_H
#define FILELISTWIDGET_H

#include <QtGui>
#include <QListWidget>
#include <QFileSystemModel>
#include <QMouseEvent>
#include <QPoint>
#include <QFile>
#include <QFileOpenEvent>
#include <QIODevice>
#include <QMenu>

class FileListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit FileListWidget(QWidget *parent = 0);

protected:
    //mouse drag and drop
//    void mousePressEvent(QMouseEvent *event);
//    void mouseMoveEvent(QMouseEvent *event);
//    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
//    void dropEvent(QDropEvent *event);

    //rcmenu popup
    void contextMenuEvent( QContextMenuEvent * event );

signals:
    
public slots:
    void filterListItemSlot(const QString &text);
protected:
    void addListItem();
    void startDrag();
private:
    QPoint              startPos;
    QListWidgetItem     *m_item;
};

#endif // FILELISTWIDGET_H
