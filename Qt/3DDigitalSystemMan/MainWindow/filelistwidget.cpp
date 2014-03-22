#include "filelistwidget.h"
#include <QApplication>
#include <QDebug>
#include <QDirModel>

FileListWidget::FileListWidget(QWidget *parent) :
    QListWidget(parent)
{
    this->setViewMode(QListWidget::IconMode );
    this->setResizeMode(QListWidget::Adjust);
    this->setMovement(QListWidget::Free);
    this->setSortingEnabled(true);
    this->setGridSize( QSize(96, 96) );
    this->setFlow(QListView::LeftToRight);
    this->setIconSize(QSize(96,84));
    this->setSpacing(40);

    this->setAcceptDrops(true);
    this->setDragEnabled(true);
//    this->setDragDropMode(QAbstractItemView::DragDrop);
    this->setEditTriggers(QAbstractItemView::SelectedClicked);



    addListItem();
}

void FileListWidget::addListItem()
{
    QString the_dir("E:/Yvi/MaxScript/MyCpt/");

    QDir dir(the_dir, "*.ms" );
    qDebug()<<dir.dirName();

    for ( uint i = 0; i < dir.count(); i++ )
    {
        m_item =  new QListWidgetItem(this);
        //m_item->setTextAlignment(Qt::AlignHCenter);
        m_item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled
                         | Qt::ItemIsDragEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable);

        QFileInfo fileInfo(the_dir+dir[i]);
        QFileIconProvider fileIcon;
        QIcon icon = fileIcon.icon(fileInfo);
        QString filename = fileInfo.fileName();
        QString fullFilename = fileInfo.absoluteFilePath();
        //qDebug() << fullFilename;
        m_item->setIcon(icon);
        m_item->setText(filename);
//        m_item->setData(Qt::AccessibleDescriptionRole, fullFilename);
//        m_item->setData(Qt::EditRole,filename);
//        m_item->setFlags(Qt::ItemIsDragEnabled);
//        m_item->setFlags(Qt::ItemIsEditable);
//        this->setCurrentItem(m_item);
    }

    this->update();
}

void FileListWidget::startDrag()
{
    QListWidgetItem *item = currentItem();
    if (item)
    {
        //if (drag->start(Qt::MoveAction) == Qt::MoveAction)
            this->window()->showMinimized();
    }
}
/*
void FileListWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        QListWidgetItem *item = static_cast<QListWidgetItem *>(this->itemAt(event->pos()));
        if(!item) return;
        item->setSelected(1);

//        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled
//                       | Qt::ItemIsDragEnabled | Qt::ItemIsEditable);

        QString the_dir("E:/Yvi/MaxScript/MyCpt/");
        //QPoint hotSpot = event->pos() - this->itemClicked( item->pos();
        QByteArray itemData;
        QDataStream dataStream(&itemData, QIODevice::WriteOnly);
        dataStream << item->text() << item->icon();

        QString fullnamepath = the_dir+item->text();
        QUrl url;
        QList<QUrl> listUrl;
        listUrl.append( url.fromLocalFile(fullnamepath) );
        QMimeData *mimeData = new QMimeData;
        mimeData->setData("text/uri-list", itemData);
        mimeData->setText(fullnamepath);
        //mimeData->setImageData(QImage(":/icon/material.png"));
        mimeData->setUrls(listUrl);

        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimeData);
        drag->setPixmap(QPixmap(":/icon/material.png"));


        //this->window()->hide();//->parentWidget()->parentWidget()->hide();
        if (drag->exec(Qt::MoveAction | Qt::CopyAction, Qt::CopyAction) == Qt::MoveAction)
        {
            qDebug() << "show";
            //this->window()->showNormal();
        }
        else
        {
            qDebug() << "hide";
            //this->window()->showMinimized();
        }


    }//leftbutton

    //QListWidget::mousePressEvent(event);
}


void FileListWidget::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() << "mouseMoveEvent";
    if (event->buttons() & Qt::LeftButton)
    {
        int distance = (event->pos() - startPos).manhattanLength();
        if (distance >= QApplication::startDragDistance())
            startDrag();
    }
    QListWidget::mouseMoveEvent(event);
}

// drag and drog
void FileListWidget::dragEnterEvent(QDragEnterEvent *event)
{
    FileListWidget *source =
            qobject_cast<FileListWidget *>(event->source());
    if (source && source != this)
    {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}
*/
void FileListWidget::dragMoveEvent(QDragMoveEvent *event)
{
    qDebug() << "dragMoveEvent";
    if (event->mimeData()->hasFormat("text/uri-list"))
    {
        qDebug() << "hasFormat";
        if (children().contains(event->source()))
        {
            qDebug() << "contains";
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }
        else
        {
            event->acceptProposedAction();
        }
    }
    else if (event->mimeData()->hasText())
    {
        this->window()->showMinimized();
        event->acceptProposedAction();
    }
    else
    {
        qDebug() << "ignore";
        event->ignore();
    }

    //QListWidget::dragMoveEvent(event);
}
/*
void FileListWidget::dropEvent(QDropEvent *event)
{


    if (event->mimeData()->hasFormat("text/uri-list"))
    {
        qDebug() << "dropEvent";
        const QMimeData *mime = event->mimeData();
        QByteArray itemData = mime->data("text/uri-list");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        QString text;
        QIcon icon;
        dataStream >> text >> icon;

        if (event->source() == this)
        {
            qDebug() << "text";
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }
        else
        {
            qDebug() << "dddddddddd";
            event->acceptProposedAction();
        }
    }
}

*/
///////////////////////////////////////////////////////////////
//rightclick popup menu at the item
void FileListWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *popMenu = new QMenu(this);
    QList<QListWidgetItem*> selItems = this->selectedItems();
    for(int i = 0; i != selItems.count(); ++i)
    {
        selItems[i]->setSelected(0);
    }
    QListWidgetItem *item = this->itemAt(mapFromGlobal(QCursor::pos()));
    if(item )
    {
        item->setSelected(1);
        popMenu->addAction(QIcon(":/icon/run.png"),tr("Run"));
        popMenu->addAction(QIcon(":/icon/edit_script.png"),tr("Edit"));
        popMenu->addAction(QIcon(":/icon/delete.png"),tr("Delete"));
        popMenu->addAction(QIcon(":/icon/moveto.png"),tr("Move to"));
        popMenu->addAction(QIcon(":/icon/favorites.png"),tr("add to favorites"));
    }
    popMenu->addAction(QIcon(":/icon/add_script.png"),tr("Add"));
    popMenu->exec(cursor().pos());
}

void FileListWidget::filterListItemSlot(const QString &text)
{
    QList<QListWidgetItem*> listItem;
    listItem = this->findItems(text,Qt::MatchFixedString | Qt::MatchContains | Qt::MatchWildcard);
    if(listItem.count() != 0)
    {
        for(int i = 0; i != this->count(); ++i)
        {
            this->item(i)->setHidden(1);

        }
        for(int i = 0; i!= listItem.count(); ++i)
        {
            listItem[i]->setHidden(0);
        }
    }
    else
    {
        for(int i = 0; i != this->count(); ++i)
        {
            this->item(i)->setHidden(1);

        }
    }

    if(text.isEmpty())
        for(int i = 0; i != this->count(); ++i)
        {
            this->item(i)->setHidden(0);

        }
}
