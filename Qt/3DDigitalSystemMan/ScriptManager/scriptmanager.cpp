#include "scriptmanager.h"


ScriptManager::ScriptManager(QWidget *parent):QListWidget(parent)
{
    this->setViewMode(QListWidget::IconMode );
    this->setResizeMode(QListWidget::Adjust);
    this->setMovement(QListWidget::Free);
    this->setSortingEnabled(true);
    this->setGridSize( QSize(64, 64) );
    this->setFlow(QListView::LeftToRight);
    this->setIconSize(QSize(32,32));
    this->setSpacing(40);

    this->setAcceptDrops(true);
    this->setDragEnabled(true);
    this->setDragDropMode(QAbstractItemView::DragDrop);
    this->setEditTriggers(QAbstractItemView::SelectedClicked);

    //add item
    setScriptsDir(QString("D:/Qt/WorkProjects/3DDigitalSystemMan/res/scripts/"));
    addListItem(m_scriptDir);
}

void ScriptManager::addListItem(const QString path)
{
    QString filter = "*.ms;*.mse;*.mzp";
    QDir dir(path, filter);
    if( !dir.isReadable() ) return;

    for ( uint i = 0; i < dir.count(); i++ )
    {
        m_item =  new QListWidgetItem(this);
        m_item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled
                         | Qt::ItemIsDragEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable);

        QFileInfo fileInfo(path+dir[i]);
        QFileIconProvider fileIcon;
        QIcon icon = fileIcon.icon(fileInfo);
        QString filename = fileInfo.fileName();
        QString fullFilename = fileInfo.absoluteFilePath();
        //qDebug() << filename << "  " << fullFilename;
        m_item->setIcon(icon);
        m_item->setText(filename);
        m_item->setData(Qt::AccessibleDescriptionRole, fullFilename);

        m_itemTextList.append( m_item->text());
    }
    this->setCurrentItem(m_item);
    this->update();
}

///////////////////////////////////////////////////////////////
//rightclick popup menu at the item
void ScriptManager::contextMenuEvent(QContextMenuEvent *event)
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
        popMenu->addAction(QIcon(":/icon/run.png"),tr("ÔËÐÐ"));
        popMenu->addAction(QIcon(":/icon/edit_script.png"),tr("±à¼­"));
        popMenu->addAction(QIcon(":/icon/delete.png"),tr("É¾³ý"));
        popMenu->addAction(QIcon(":/icon/moveto.png"),tr("ÒÆ¶¯"));
        popMenu->addAction(QIcon(":/icon/favorites.png"),tr("ÊÕ²Ø"));
    }
    popMenu->addAction(QIcon(":/icon/add_script.png"),tr("Ìí¼Ó"));
    popMenu->exec(cursor().pos());
}

void ScriptManager::mouseMoveEvent(QMouseEvent *event)
{
    QListWidgetItem *item = static_cast<QListWidgetItem *>(this->itemAt(event->pos()));
    if(!item)
    {
        event->ignore();
        return;
    }
    if(!item->isSelected())
    {
        return;
    }

    m_startPos = event->pos();

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << item->text() << item->icon();// << mapToParent(QPoint(0,0));

    QString fullfilename = m_scriptDir + item->text();
    QUrl url;
    QList<QUrl> urlList;
    urlList.append( url.fromLocalFile(fullfilename) );
    QMimeData *mimeData = new QMimeData;
    mimeData->setData("text/uri-list", itemData);
    mimeData->setText(item->text());
    mimeData->setUrls(urlList);
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(QPixmap(":/icon/file_3dsmax_ms.ico"));

    drag->start();

}

void ScriptManager::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->source() != this)
    {
        //event->ignore();
        return;
    }

    QPoint point = event->pos() - m_startPos;
     if ( point.manhattanLength() > 50 )
     {
         // the mouse has moved more than 50 pixels since the oldPosition
         this->window()->showMinimized();
     }
     else
         return;
}

void ScriptManager::dropEvent(QDropEvent *event)
{
    ScriptManager *source = qobject_cast<ScriptManager *>(event->source());
    if (source && source == this)
    {
        event->ignore();
        return;
    }

    if( event->mimeData()->hasUrls() )
    {

        QString localFile;
        QRegExp rx("\\.(ms|mse|mzp)$", Qt::CaseInsensitive);
        foreach(QUrl url, event->mimeData()->urls())
        {
            localFile = url.toLocalFile();
            if( rx.indexIn(localFile) >= 0 )
            {
                event->accept();
                event->setDropAction(Qt::CopyAction);
                //add item
                QFileInfo fileInfo(localFile);
                QFileIconProvider fileIcon;
                QIcon icon = fileIcon.icon(fileInfo);
                QString filename = fileInfo.fileName();
                QString fullFilename = fileInfo.absoluteFilePath();
                qDebug() << filename << "  " << fullFilename;
                m_item->setIcon(icon);
                m_item->setText(filename);
                m_item->setData(Qt::AccessibleDescriptionRole, fullFilename);

                m_itemTextList.append( m_item->text());

                return;
            }
            else
                event->ignore();
        }
    }
    else
        event->ignore();
}

//Drag file(s) into this QListWidget from explorer.
void ScriptManager::dragEnterEvent(QDragEnterEvent *event)
{
    if( event->mimeData()->hasUrls() )
    {
        QString localFile;
        QRegExp rx("\\.(ms|mse|mcr|mzp)$", Qt::CaseInsensitive);
        foreach(QUrl url, event->mimeData()->urls())
        {
            localFile = url.toLocalFile();
            if( rx.indexIn(localFile) >= 0 )
            {
                event->accept();
                return;
            }
            else
                event->ignore();
        }
    }
    else
        event->ignore();
}

// search type key words and update the script files list
void ScriptManager::OnFilterListItemSlot(const QString &text)
{
     if(text ==  tr("ËÑË÷¹Ø¼ü×Ö") ) return;
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

void ScriptManager::OnIcoScaleSlot(int size)
{
    this->setIconSize( QSize(size, size) );
    this->setGridSize( QSize(size*2, size*2) );
}
