#include "modelmanager.h"
#include <QDebug>
#include <QMessageBox>

ModelManager::ModelManager(QWidget *parent) :
    QListWidget(parent)
{
    this->setViewMode(QListWidget::IconMode );
    this->setFlow(QListView::LeftToRight);
    this->setUniformItemSizes(true);
    this->setResizeMode(QListWidget::Adjust);//Fixed
    this->setMovement(QListWidget::Snap);//Snap
    //this->setTextElideMode(Qt::ElideRight);
    this->setSortingEnabled(true);
    //this->setGridSize( QSize(ICON_W, ICON_H) );
    this->setIconSize(QSize(ICON_W, ICON_H));
    this->setSpacing(10);

    this->setWordWrap(true);
    //this->setWrapping(true);
    this->setAcceptDrops(true);
    this->setDragEnabled(true);
    this->setDragDropMode(QAbstractItemView::DragDrop);
    this->setEditTriggers(QAbstractItemView::SelectedClicked);

    m_iconHight = 32;
    m_iconWidth = 32;
    //initalized list
    InitListItem();
}


ModelManager::~ModelManager()
{
}

bool ModelManager::CreateConnectDB(QSqlDatabase& db)
{
    if(QSqlDatabase::contains("INILISTWIDGET"))
        db = QSqlDatabase::database("INILISTWIDGET");
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE", "INILISTWIDGET");
        db.setDatabaseName(MODEL_SYSTEM_DB);
    }

    if(!db.isOpen())
    {
        if( !db.open() )
        {
            QMessageBox::warning(this, tr("connect error"), tr("Error:")+ db.lastError().text(), QMessageBox::Ok);
            return false;
        }
    }

    QStringList tableList = db.tables();
    QSqlQuery query(db);
    if(!tableList.contains(MODEL_SYSTEM_TABEL))
    {
        QString createTable = QString("create table %1 (id INTEGER PRIMARY KEY AUTOINCREMENT,"
             "typeName varchar(64), modelName varchar(64) unique, "
             "modelPath  varchar(128), thumbnailPath varchar(128)  )").arg(MODEL_SYSTEM_TABEL);
        if ( !query.exec(createTable) )
        {
            QMessageBox::warning(this, tr("create table error"), tr("Error:")+query.lastError().text(), QMessageBox::Ok);
            return false;
        }
    }

    return true;
}

void ModelManager::InitListItem()
{
    this->clear();
    QSqlDatabase db;
    if( !CreateConnectDB(db) )
    {
        QMessageBox::warning(this, tr("connect error"), tr("Error:")+db.lastError().text(), QMessageBox::Ok);
        return;
    }
    //if ( m_modelListDB->isValid() )
    {
        QSqlQuery query = QSqlQuery::QSqlQuery(db);

        bool isok = query.exec( QString("select * from %1;").arg(MODEL_SYSTEM_TABEL) );
        if (!isok)
        {
            QMessageBox::warning(this, tr("open table error"), tr("Error:")+db.lastError().text(), QMessageBox::Ok);
            db.close();
            db.removeDatabase(MODEL_SYSTEM_DB);
            return;
        }
        QSqlRecord recd = query.record();
        //int fieldCount = recd.count();
        /*真正的不应该查询所有的记录集的,只查询上一次关闭系统前选中的分类,只查询该分类字段下的记录集*/
        while( query.next() )
        {
            //for(int i=0; i < fieldCount; i++)
            {
                //0,ID.  1,type.  2,model. 3,maxFilename. 4,thumbFilename.
                //qDebug() << query.value(2).toString() << query.value(3).toString() << query.value(4).toString();
                QString modelName = query.value(2).toString();
                QString maxFile = query.value(3).toString();
                QString thumbFile = query.value(4).toString();
                AddItem(modelName, maxFile, thumbFile);
            }
        }

        query.clear();
        db.close();
        db.removeDatabase(MODEL_SYSTEM_DB);
    }
}

void ModelManager::AddItem(const QString& modelName, const QString& maxFilename, const QString& thumbFilename)
{
    QListWidgetItem* item =  new QListWidgetItem(this);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignBottom | Qt::AlignAbsolute);
    item->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled
                 | Qt::ItemIsDragEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable);

    QPixmap pixmap(thumbFilename);
    QIcon itemIcon( pixmap.scaled( QSize(ICON_W, ICON_H) ) );
    item->setIcon( itemIcon );
    this->setIconSize( QSize(m_iconWidth*1.6, m_iconHight) );
    item->setSizeHint( QSize(m_iconWidth*1.6, m_iconHight+24) );

    if(modelName.isEmpty())
    {
        QFileInfo fileInfo(maxFilename);
        item->setText(fileInfo.fileName());
    }
    else
        item->setText(modelName);

    m_itemTextList.append( modelName );
}

void ModelManager::OnFilterListItemSlot(const QString &text)
{
    if(text ==  tr("搜索关键字") ) return;
    QList<QListWidgetItem*> listItem;
    listItem = this->findItems(text,Qt::MatchFixedString | Qt::MatchContains | Qt::MatchWildcard);
    if(listItem.count() != 0)
    {
        for(int i = 0; i != this->count(); ++i)
        {
            this->item(i)->setHidden(1);
        }
        foreach(QListWidgetItem *item, listItem)
        {
            item->setHidden(0);
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

void ModelManager::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *popMenu = new QMenu(this);
    QList<QListWidgetItem*> selItems = this->selectedItems();
    for(int i = 0; i != selItems.count(); ++i)
    {
        selItems[i]->setSelected(0);
    }

    QAction *openAction = popMenu->addAction(QIcon(":/icon/run.png"),tr("打开"));
    popMenu->addSeparator();
    QAction *editAction = popMenu->addAction(QIcon(":/icon/edit_script.png"),tr("编辑"));
    QAction *deleteAction = popMenu->addAction(QIcon(":/icon/delete.png"),tr("删除"));
    QMenu* movetoSubMenu = popMenu->addMenu(QIcon(":/icon/moveto.png"),tr("移动"));
        QList<QAction*> actions;
        for(int i = 0; i < 10; ++i)
            actions.append(new QAction(tr("item%1").arg(i), this));
        movetoSubMenu->addActions( actions );
    QAction *refreshAction = popMenu->addAction(QIcon(":/icon/refresh.png"),tr("刷新"));
    popMenu->addSeparator();
    m_addAction = popMenu->addAction(QIcon(":/icon/add_script.png"),tr("添加"));
    QAction *favoritesAction = popMenu->addAction(QIcon(":/icon/favorites.png"),tr("收藏"));

    QListWidgetItem *item = this->itemAt(mapFromGlobal(QCursor::pos()));
    if( item )
    {
        item->setSelected(1);
        openAction->setEnabled(true);
        editAction->setEnabled(true);
        deleteAction->setEnabled(true);
        movetoSubMenu->setEnabled(true);
        favoritesAction->setEnabled(true);

        actions[1]->setDisabled(true);
    }
    else
    {
        openAction->setDisabled(true);
        editAction->setDisabled(true);
        deleteAction->setDisabled(true);
        movetoSubMenu->setDisabled(true);
        favoritesAction->setDisabled(true);
    }
    m_addAction->setEnabled(true);

    connect(deleteAction, SIGNAL( triggered() ), this, SLOT( OnPopMenuDelModelSlot() ));
    //connect(m_addAction, SIGNAL( triggered() ), this->window(), SLOT( OnShowAddMaxModelDlgSlot() ));
    connect(refreshAction, SIGNAL( triggered() ), this, SLOT( OnPopMenuRefleshSlot() ));

    popMenu->exec(cursor().pos());
}

void ModelManager::mousePressEvent(QMouseEvent *event)
{
    QListWidgetItem *item = static_cast<QListWidgetItem *>(this->itemAt(event->pos()));
    if(!item)
    {
        QList<QListWidgetItem*> listItems = this->selectedItems();
        foreach(QListWidgetItem *_item, listItems)
        {
            _item->setSelected(false);
        }
        return;
    }

    //select form database
    //需要查询数据库获取文件路径
    QSqlDatabase db;
    if(QSqlDatabase::contains("DRAG"))
        db = QSqlDatabase::database("DRAG");
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE", "DRAG");
        db.setDatabaseName(MODEL_SYSTEM_DB);
        db.open();
    }
    QSqlDatabase::database().transaction();
    QSqlQuery query(db);
    QString sqlStr = QString("SELECT * FROM %1 WHERE modelName='%2';").arg(MODEL_SYSTEM_TABEL).arg(item->text());
    bool ok = query.exec(sqlStr);
    if(ok && query.next())
    {
        m_maxFile = query.value(3).toString();
        m_thumbFile = query.value(4).toString();
    }
    else
    {
        event->ignore();
        return;
    }
    QSqlDatabase::database().commit();

    QListWidget::mousePressEvent(event);
}

void ModelManager::mouseMoveEvent(QMouseEvent *event)
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
    dataStream << item->text() << item->icon();

    QUrl url;
    QList<QUrl> urlList;
    urlList.append( url.fromLocalFile(m_maxFile) );
    QMimeData *mimeData = new QMimeData;
    mimeData->setData("text/uri-list", itemData);
    mimeData->setText(item->text());
    mimeData->setUrls(urlList);
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(QPixmap(m_thumbFile).scaled(QSize(m_iconWidth,m_iconHight), Qt::IgnoreAspectRatio) );

    drag->start();
}

void ModelManager::dragMoveEvent(QDragMoveEvent *event)
{
    QPoint point = event->pos() - m_startPos;

     if ( point.manhattanLength() > 50 )
     {
         // the mouse has moved more than 50 pixels since the oldPosition
         this->window()->showMinimized();
     }
}

void ModelManager::dragEnterEvent(QDragEnterEvent *event)
{
    if( event->mimeData()->hasUrls() )
    {
        QString localFile;
        QRegExp rx("\\.(max)$", Qt::CaseInsensitive);
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

void ModelManager::dropEvent(QDropEvent *event)
{
    ModelManager *source = qobject_cast<ModelManager *>(event->source());
    if (source && source == this)
    {
        event->ignore();
        return;
    }

    if( event->mimeData()->hasUrls() )
    {
        QString localFile;
        QRegExp rx("\\.(max)$", Qt::CaseInsensitive);
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

//分类列表中切换分类时,同时更新模型管理列表
void ModelManager::OnUpdateModelViewSlot(const QList<QStringList> &list)
{
    if(list.size() == 0)
    {
        this->clear();
        return;
    }
    this->clear();
    foreach(QStringList l, list)
    {
        //QString typeName = l[0];
        QString modelName = l[1];
        QString maxFilename = l[2];
        QString thumbFilename = l[3];
        AddItem(modelName, maxFilename, thumbFilename);
    }
    this->update();
}

//图标缩放slider改变,同时修改模型管理图标大小
void ModelManager::OnIcoScaleSlot(int size)
{
    m_iconHight = size;
    m_iconWidth = size;
    this->setIconSize( QSize(1.6*size, size) );
    //this->setGridSize( QSize(2*size+10, size+10) );
    for( int i = 0; i < this->count(); ++i)
    {
        this->item(i)->setSizeHint( QSize(size*1.6, size+24) );
    }
}

void ModelManager::OnPopMenuDelModelSlot()
{
    QList<QListWidgetItem*> listItems = this->selectedItems();
    if( listItems.size() == 0 ) return;
    foreach(QListWidgetItem *_item, listItems)
    {
        qDebug() << "delete item: " << _item->text();

        delete this->takeItem(this->row(_item));
        this->update();
    }
}

void ModelManager::OnItemClickedSlot(QListWidgetItem *item)
{
    qDebug() << item->text();
}

void ModelManager::OnPopMenuRefleshSlot()
{
    emit updateItemIconSignal();//发送信号给slider通知其需要发送当前的Icon值到modellist来设置图标大小
    emit getCurrentModelTypeSignal();//发送信号给模型分类列表,得到当前选中的分类类型,并返回信号给当前的模型管理列表,从当前的分类类型中查询数据库,重新加载上来.
}

//接受分类列表发回来的信号,并重新更新模型列表
void ModelManager::OnReFreshCurrentModelTypeSlot(const QString &queryKey)
{
    QSqlDatabase db;
    if(QSqlDatabase::contains("REFRESHCURRENTTYPE"))
        db = QSqlDatabase::database("REFRESHCURRENTTYPE");
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE", "REFRESHCURRENTTYPE");
        db.setDatabaseName(MODEL_SYSTEM_DB);
    }

    if(!db.isOpen())
    {
        if( !db.open() )
        {
            QMessageBox::warning(this, tr("connect error"), tr("Error:")+ db.lastError().text(), QMessageBox::Ok);
            return;
        }
    }
    QString strSql = QString("SELECT * from %1").arg(MODEL_SYSTEM_TABEL);
    QSqlQuery query(strSql, db);
    int fieldNo = query.record().indexOf( "typeName" );
    this->clear();
    while ( query.next() )
    {
        QString typeName = query.value(fieldNo).toString();
        if(typeName == queryKey)
        {
            QString modelName = query.value(fieldNo+1).toString();
            QString modelPath = query.value(fieldNo+2).toString();
            QString modelThumb = query.value(fieldNo+3).toString();
            AddItem( modelName, modelPath, modelThumb );
        }
    }
    this->update();
    query.clear();
}

//获取slider当前Icon的值
void ModelManager::OnGetScaleIcoSizeSlot(const int &size)
{
    m_iconHight = size;
    m_iconWidth = size;
    qDebug() << size;
}


