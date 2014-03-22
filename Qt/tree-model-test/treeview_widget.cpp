#include "treeview_widget.h"
#include <QAction>
#include <QMenu>
#include <QContextMenuEvent>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QDebug>

TreeViewWidget::TreeViewWidget(QWidget *parent) :
    QTreeView(parent)
{
    setDragEnabled(true);
    setAcceptDrops(true);
    setDropIndicatorShown(true);
    setDragDropMode(QAbstractItemView::InternalMove);
    setSelectionMode(QAbstractItemView::SingleSelection);
}

void TreeViewWidget::contextMenuEvent(QContextMenuEvent */*event*/)
{
    QMenu *popMenu = new QMenu(this);
    QAction *insertRowAct = popMenu->addAction(tr("&Insert Row"));
    QAction *removeRowAct = popMenu->addAction(tr("&Remove Row"));
    popMenu->addSeparator();
    QAction* newChildAct = popMenu->addAction(tr("&New Child"));

    int _x = mapFromGlobal(QCursor::pos()).x();
    int _y = mapFromGlobal(QCursor::pos()).y() - this->header()->height();// 如果有表头,需要减去表头高度

    QModelIndex index = this->indexAt( QPoint(_x, _y) );

    if( !index.isValid() )
    {
        this->clearSelection();
        this->selectionModel()->setCurrentIndex(QModelIndex(), QItemSelectionModel::Clear);
        removeRowAct->setEnabled(false);
        newChildAct->setEnabled(false);
    }

    connect(insertRowAct, SIGNAL( triggered() ), this, SLOT( insertRow() ));
    connect(removeRowAct, SIGNAL( triggered() ), this, SLOT( removeRow() ));
    connect(newChildAct, SIGNAL( triggered() ), this, SLOT( insertChild() ));
    popMenu->exec(cursor().pos());
}

void TreeViewWidget::insertChild()
{
    QModelIndex index = this->selectionModel()->currentIndex();
    QAbstractItemModel *model = this->model();

    if (model->columnCount(index) == 0)
    {
        if (!model->insertColumn(0, index))
            return;
    }

    if (!model->insertRow(0, index))
        return;

    for (int column = 0; column < model->columnCount(index); ++column)
    {
        QModelIndex child = model->index(0, column, index);
        model->setData(child, QVariant(QString("[No data %1]").arg(index.internalId()+1)), Qt::EditRole);
        if (!model->headerData(column, Qt::Horizontal).isValid())
            model->setHeaderData(column, Qt::Horizontal, QVariant("[No header]"),Qt::EditRole);
    }

    this->selectionModel()->setCurrentIndex(model->index(0, 0, index),
                                            QItemSelectionModel::ClearAndSelect);
}

void TreeViewWidget::insertRow()
{
    QModelIndex index = this->selectionModel()->currentIndex();
    QAbstractItemModel *model = this->model();

    int first = 0;
    if( index.row() != -1 )
        first = index.row()+1;
    else
        first = 1;

//    qDebug() << "index.row(): " << index.row() << index.row()+1;

    if (!model->insertRow(first, index.parent()))
    {
        qDebug() << index.row()+1 << "insert row failed.";
        return;
    }

    //当前树只有一列,下面针对多列操作的,可以不需要
//    for (int column = 0; column < model->columnCount(index.parent()); ++column)
//    {
//        QModelIndex child = model->index(index.row()+1, column, index.parent());
//        model->setData(child, QVariant(QString("[No data %1]").arg(index.internalId()+1)), Qt::EditRole);
//    }
}

void TreeViewWidget::removeRow()
{
    QModelIndex index = this->selectionModel()->currentIndex();
    if( !index.isValid() ) return;

    QAbstractItemModel *model = this->model();
    qDebug() << "delete: " << index.row() << index.internalId();
    if (model->removeRow(index.row(), index))
        qDebug() << "remoce row successful.";// << index.internalId();
}

//void TreeViewWidget::dragEnterEvent(QDragEnterEvent *event)
//{
//     event->acceptProposedAction();
//}

//void TreeViewWidget::dropEvent(QDropEvent *event)
//{
//    qDebug("I am here");
//    event->acceptProposedAction();
//}

//void TreeViewWidget::dragMoveEvent(QDragMoveEvent *event)
//{
//}

