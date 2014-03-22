#ifndef FILE_TREEVIEW_H
#define FILE_TREEVIEW_H

#include <QTreeView>

class QFileSystemModel;

class FileTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit FileTreeView(QWidget *parent = 0);
    
signals:
    
public slots:
    
private:
    void initialize();

    QFileSystemModel        *m_fsModel;
    QString                 m_currentPath;
};

#endif // FILE_TREEVIEW_H
