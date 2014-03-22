#ifndef TREEVIEW_WIDGET_H
#define TREEVIEW_WIDGET_H

#include <QTreeView>

class QAction;
class QMenu;
class QContextMenuEvent;
class QDrag;

class TreeViewWidget : public QTreeView
{
    Q_OBJECT
public:
    explicit TreeViewWidget(QWidget *parent = 0);
    
signals:
    
private slots:
    void insertChild();
    void insertRow();
    void removeRow();

protected:
    void contextMenuEvent(QContextMenuEvent *event);
//    void dragEnterEvent(QDragEnterEvent *event);
//    void dropEvent(QDropEvent *event);
//    void dragMoveEvent(QDragMoveEvent *event);

};

#endif // TREEVIEW_WIDGET_H
