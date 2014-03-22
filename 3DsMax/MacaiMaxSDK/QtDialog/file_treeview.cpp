#include "file_treeview.h"

#include <QFileSystemModel>


FileTreeView::FileTreeView(QWidget *parent) :
    QTreeView(parent)
{
    initialize();
}

void FileTreeView::initialize()
{
    m_fsModel = new QFileSystemModel(this);
    m_fsModel->setRootPath(QDir::currentPath());
    this->setModel(m_fsModel);
}
