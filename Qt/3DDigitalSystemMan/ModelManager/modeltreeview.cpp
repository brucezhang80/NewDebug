#include "modeltreeview.h"
#include "treemodel.h"

#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include <QTextCodec>
#include <QTextStream>
#include <QFileInfo>
#include <QDir>
#include <QSettings>
#include <QMenu>

ModelTreeView::ModelTreeView(QWidget *parent) :
    QTreeView(parent)
{
    m_modelFile = "model";
    load(m_modelFile);
}

ModelTreeView::~ModelTreeView()
{
    save(m_modelFile);
    saveState();
}

void ModelTreeView::queryModelType(const QString &queryKey, QList<QStringList> &modelList)
{
    QSqlDatabase db;
    if(QSqlDatabase::contains("QUERYMODELTYPE"))
        db = QSqlDatabase::database("QUERYMODELTYPE");
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE", "QUERYMODELTYPE");
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
    while ( query.next() )
    {
        QString typeName = query.value(fieldNo).toString();
        if(typeName == queryKey)
        {
            QString modelName = query.value(fieldNo+1).toString();
            QString modelPath = query.value(fieldNo+2).toString();
            QString modelThumb = query.value(fieldNo+3).toString();

            QStringList list;
            list << typeName << modelName << modelPath << modelThumb;
            modelList.append(list);
        }
    }
    query.clear();
}

void ModelTreeView::mousePressEvent(QMouseEvent *event)
{
    if( event->button() == Qt::LeftButton)
    {
        QModelIndex index = static_cast<QModelIndex>(this->indexAt(event->pos()));
        if(index.isValid())
        {
            if ( index.child(0,0).isValid() )
            {
                if(this->isExpanded(index))
                    this->collapse(index);
                else
                    this->expand(index);
            }
            if ( this->currentIndex() == index )
                return;
            this->setCurrentIndex(index);
            QString curItem = index.data().toString();

            emit itemClickedSignal(curItem);

            QList<QStringList> modelList;
            queryModelType(curItem, modelList);

            emit updateModelViewSignal(modelList);
        }
    }
}

//void ModelTreeView::mouseDoubleClickEvent(QMouseEvent *event)
//{

//}

void ModelTreeView::contextMenuEvent(QContextMenuEvent *event)
{
    QModelIndex index = this->currentIndex();
    //QString itemText = this->model()->data(index).toString();
    this->setCurrentIndex(indexAt(event->pos()));

    QMenu *popMenu = new QMenu(this);
    //QAction *newAction = popMenu->addAction(QIcon(":/icon/run.png"),tr("�½�"));
    QAction *newTypeAction = popMenu->addAction(QIcon(":/icon/run.png"),tr("�½�����"));
    popMenu->addSeparator();
    QAction *movetoAction = popMenu->addAction(QIcon(":/icon/run.png"),tr("�ƶ�"));
    QAction *renameAction = popMenu->addAction(QIcon(":/icon/run.png"),tr("������"));
    QAction *deleteAction = popMenu->addAction(QIcon(":/icon/run.png"),tr("ɾ��"));
    popMenu->addSeparator();
    QAction *refreshAction = popMenu->addAction(QIcon(":/icon/refresh.png"),tr("ˢ��"));

    QModelIndex indexChild = this->currentIndex().child(0, 0);
    bool ok = this->model()->data(indexChild).toBool();
    if(!ok)//��ǰ�����Ҷ�ӽڵ���
    {
        newTypeAction->setEnabled(true);
        newTypeAction->setEnabled(true);
        movetoAction->setEnabled(true);
        renameAction->setEnabled(true);
        deleteAction->setEnabled(true);
    }
    else
    {
        movetoAction->setDisabled(true);
        deleteAction->setDisabled(true);
    }

    if (this->model()->data(index).isNull())
    {
        //newAction->setDisabled(true);
        //newTypeAction->setDisabled(true);
        movetoAction->setDisabled(true);
        renameAction->setDisabled(true);
        deleteAction->setDisabled(true);

    }

    connect(deleteAction, SIGNAL( triggered( ) ), this, SLOT( OnDeleteItemSlot( ) ));
    connect(newTypeAction, SIGNAL( triggered( ) ), this, SLOT( OnInsertItemSlot( ) ));
    connect(refreshAction, SIGNAL( triggered( ) ), this, SLOT( OnRefreshItemSlot( ) ));
    connect(renameAction, SIGNAL( triggered( ) ), this, SLOT( OnRenameCurrentItemSlot( ) ));

    popMenu->exec(cursor().pos());
}

void ModelTreeView::load(const QString &filename)
{
    QFile modelFile(filename);
    if(!modelFile.exists())
    {
        QMessageBox::warning(this->window(), tr("����"), tr("ģ�ͷ��������ļ���ʧ��������Ĭ�Ϸ��ࡣ"), QMessageBox::Ok);
        if(!create(filename))
        {
            QMessageBox::warning(this->window(), tr("����"), tr("�������ݱ���ʧ��, �Ƿ��㹻��Ȩ��д���������?"), QMessageBox::Ok);
            return;
        }
    }
    if( !modelFile.open(QIODevice::ReadOnly) )
    {
        QMessageBox::warning(this->window(), tr("����"), tr("û��Ȩ�޶�ȡ�����ļ���"), QMessageBox::Ok);
        return;
    }

    QTextStream stream(modelFile.readAll(),QIODevice::ReadOnly);
    stream.setCodec( QTextCodec::codecForName("SYSTEM") );
    QByteArray byteArray;
    byteArray.append(stream.readAll());
    modelFile.close();

    QStringList headers;
    headers << tr("����") << tr("����");
    m_model= new TreeModel(headers, byteArray);
    this->setModel(m_model);
    setColumnHidden(1, true);
    restoreState();
}

bool ModelTreeView::create(const QString &filename)
{
    QFile file(filename);
    if(!file.exists())
    {
        //��ʽ��AppendΪ׷�ӣ�WriteOnly��ReadOnly
        if (!file.open( QIODevice::WriteOnly|QIODevice::Text ))
        {
            return false;
        }
        QTextStream out(&file);
        QStringList textList;
        QByteArray byteArr;
        textList << tr("����\n") << tr("    ����\n") << tr("    Ů��\n") << tr("    С��\n") << tr("    ����\n") << tr("    ����\n")
             << tr("����\n") << tr("    ����\n") << tr("    ½��\n") << tr("    ˮ��\n") << tr("    ����\n") << tr("    ����\n") << tr("    ����\n")
             << tr("ֲ��\n") << tr("    ��\n") << tr("    ��ľ\n") << tr("    ����\n") << tr("    ����\n") << tr("    ����\n") << tr("    ����\n")
             << tr("�ƾ�\n") << tr("    ������\n") << tr("    ����\n") << tr("    ̨��\n") << tr("    �ڵ�\n") << tr("    ·��\n")<< tr("    ��դ��\n")<< tr("    ����\n")
             << tr("����\n") << tr("    ����\n") << tr("    ����\n") << tr("    ԰��\n") << tr("    ��ʩ\n") << tr("    �Ŵ�\n") << tr("    ����\n") << tr("    ����\n")
             << tr("��ͨ����\n") << tr("    ������\n") << tr("    �ǻ�����\n") << tr("    ��ֻ\n") << tr("    �ɻ�\n") << tr("    ����\n")
             << tr("�����豸\n") << tr("    �ճ�\n") << tr("    �칫\n") << tr("    ����\n") << tr("    ����\n") << tr("    ҽ��\n") << tr("    ��Ϸ\n") << tr("    ����\n")
             << tr("��𹹼�\n") << tr("    ��˿\n") << tr("    ��˿��\n") << tr("    �ܼ�\n") << tr("    ���\n") << tr("    �о��\n") << tr("    ����\n")
             << tr("�ҵ��ղ�\n") << tr("    ����\n") << tr("    ����\n") << tr("    ����\n") << tr("    �ƾ�\n") << tr("    �Ҿ�\n") << tr("    СƷ\n") << tr("    ��ͨ\n");

        foreach(QString text, textList)
            byteArr.append(text);
        out<< byteArr <<endl;
        out.flush();
        file.close();
    }

    return true;
}

void ModelTreeView::OnRenameCurrentItemSlot()
{
    QModelIndex index = this->selectionModel()->currentIndex();
    this->edit(index);
}

void ModelTreeView::OnCurrentModelTypeSlot()
{
    QModelIndex curIndex = this->currentIndex();
    QString curItem = curIndex.data().toString();
    emit currentModelTypeSignal(curItem);
}

//ȡ��ģ�ͷ������Ľڵ�,�����͵�"ģ����ӶԻ���,���Գ�ʼ��ģ�ͷ�������"
//�ò���Ϣ��Ҫ"���ģ�ͶԻ���"�������ź�,�Ա㵱ǰ�ܷ�����Ϣ���ظ�"ģ����ӶԻ���"
void ModelTreeView::OnSetComboBoxModelTypeDataSlot()
{
    bool isParent = true;
    QModelIndex rootIndex = QModelIndex();
    for (int row = 0; row < m_model->rowCount(rootIndex); ++row)
    {
        QModelIndex parentIndex = m_model->index(row, 0, rootIndex);
        QString itemText = parentIndex.data(Qt::DisplayRole).toString();
        emit setComboBoxModelTypeDateSignal(itemText, true);

        travelTree(parentIndex, isParent);
    }
}

void ModelTreeView::travelTree(QModelIndex &parentIndex, bool bParent)
{
    if( parentIndex.isValid() )
    {
        if(this->model()->hasChildren(parentIndex))
        {
            for(int row = 0; row < this->model()->rowCount(parentIndex); ++row)
            {
                QModelIndex childIndex = parentIndex.child(row, 0);
                QString itemText = childIndex.data(Qt::DisplayRole).toString();

                if(model()->hasChildren(childIndex))
                {
                    bParent = true;
                    emit setComboBoxModelTypeDateSignal(itemText, bParent);
                    travelTree(childIndex, bParent);
                }
                else
                {
                    bParent = false;
                    emit setComboBoxModelTypeDateSignal(itemText, bParent);
                }
            }
        }
    }
}

void ModelTreeView::save(const QString &filename)
{
    if( this->model()->rowCount(QModelIndex()) == 0 )
        return;
    QFile modelFile(filename);
    //��ʽ��AppendΪ׷�ӣ�WriteOnly��ReadOnly
    if (!modelFile.open( QIODevice::WriteOnly|QIODevice::Text ))
    {
        QMessageBox::critical(NULL, "��ʾ", "�޷������ļ�");
        return ;
    }
    QTextStream out(&modelFile);

    QByteArray byteArr;
    QModelIndex rootIndex = QModelIndex();
    for (int row = 0; row < m_model->rowCount(rootIndex); ++row)
    {
        QString space = "    ";
        QModelIndex parentIndex = m_model->index(row, 0, rootIndex);
        QString itemText =  parentIndex.data(Qt::DisplayRole).toString() + "\n";
        byteArr.append(itemText);
        saveChild(parentIndex, &byteArr, space);
    }

    out<< byteArr <<endl;
    out.flush();
    modelFile.close();
}

void ModelTreeView::saveChild(QModelIndex &parentIndex, QByteArray *byteArray, QString & space)
{
    if( parentIndex.isValid() )
    {
        if(this->model()->hasChildren(parentIndex))
        {

            for(int row = 0; row < this->model()->rowCount(parentIndex); ++row)
            {
                QModelIndex childIndex = parentIndex.child(row, 0);
                QString itemText =  space + childIndex.data(Qt::DisplayRole).toString() + "\n";
                (*byteArray).append(itemText);

                if(model()->hasChildren(childIndex))
                {
                    space+="    ";
                    saveChild(childIndex, byteArray, space);
                    space.remove(0, 4);
                }
            }
        }
    }
}

void ModelTreeView::loadTree(const QByteArray &treeArray)
{
    QStringList headers;
    headers << tr("����") << tr("����");
    m_model = new TreeModel(headers, treeArray);
    this->setModel(m_model);
    setColumnHidden(1, true);
    restoreState();
}

void ModelTreeView::saveState()
{
    if( this->model()->rowCount(QModelIndex()) == 0 ) return;

    QStringList List;
    // prepare list
    // PS: getPersistentIndexList() function is a simple `return this->persistentIndexList()` from TreeModel model class
    foreach (QModelIndex index, m_model->getPersistentIndexList())
    {
        if (this->isExpanded(index))
        {
            List << index.data(Qt::DisplayRole).toString();
        }
    }
    // save list
    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.beginGroup("ModelTree");
    //set to other chars
    settings.setIniCodec( QTextCodec::codecForName("SYSTEM") );
    settings.setValue("ExpandedItems", QVariant::fromValue(List));
    settings.endGroup();
}

void ModelTreeView::restoreState()
{
    QStringList List;
    // get list
    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.beginGroup("ModelTree");
    settings.setIniCodec( QTextCodec::codecForName("SYSTEM") );
    List = settings.value("ExpandedItems").toStringList();
    settings.endGroup();

    foreach (QString item, List)
    {
        // search 'item' text in model
        QModelIndexList Items = m_model->match(m_model->index(0, 0), Qt::DisplayRole, QVariant::fromValue(item));
        if (!Items.isEmpty())
        {
            // Information: with this code, expands ONLY first level in QTreeView
            this->setExpanded(Items.first(), true);
        }
    }
}

void ModelTreeView::OnDeleteItemSlot()
{
    QModelIndex index = this->selectionModel()->currentIndex();
    if( index.isValid())
    {
        int currentRow = index.row();
        const QModelIndex parent = index.parent();
        //QAbstractItemModel *model = this->model();
        qDebug() << currentRow << parent.data().toString();
        if ( m_model->removeRow(currentRow, parent) )
        {
            qDebug() << "delete successed.";
        }
        else
        {
            qDebug() << "delete failed.";
        }
    }
}

void ModelTreeView::OnInsertItemSlot()
{
    QModelIndex index = this->selectionModel()->currentIndex();
    //QAbstractItemModel *model = this->model();

    if (!m_model->insertRow(0, index))
        return;

    this->expand(index);
    for (int row = 0; row < m_model->rowCount(index); ++row)
    {
        QModelIndex child = m_model->index(0, 0, index);
        m_model->setData(child, QVariant("[������]"), Qt::EditRole);
        this->setCurrentIndex(child);
        //if (!m_model->headerData(0, Qt::Horizontal).isValid())
            //m_model->setHeaderData(0, Qt::Horizontal, QVariant("[������]"),Qt::EditRole);
    }
}

void ModelTreeView::OnRefreshItemSlot()
{
    load(m_modelFile);
}

void ModelTreeView::OnInsertSubItemSlot()
{
    QModelIndex index = this->selectionModel()->currentIndex();
    //QAbstractItemModel *model = view->model();

    if (m_model->columnCount(index) == 0)
    {
        if (!m_model->insertColumn(0, index))
        return;
    }

    if (!m_model->insertRow(0, index))
     return;

    for (int column = 0; column < m_model->columnCount(index); ++column)
    {
        QModelIndex child = m_model->index(0, column, index);
        m_model->setData(child, QVariant("[������]"), Qt::EditRole);
        if (!m_model->headerData(column, Qt::Horizontal).isValid())
        m_model->setHeaderData(column, Qt::Horizontal, QVariant("[������]"),
        Qt::EditRole);
    }

    this->selectionModel()->setCurrentIndex(m_model->index(0, 0, index),
                                            QItemSelectionModel::ClearAndSelect);
}


