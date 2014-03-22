#include "scripttreeview.h"
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

ScriptTreeView::ScriptTreeView(QWidget *parent) :
    QTreeView(parent)
{
    m_scriptFile = ":/data/script.txt";
    load(m_scriptFile);
}

ScriptTreeView::~ScriptTreeView()
{
    saveState();
}

void ScriptTreeView::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *popMenu = new QMenu(this);
    //QAction *newAction = popMenu->addAction(QIcon(":/icon/run.png"),tr("新建"));
    QAction *newTypeAction = popMenu->addAction(QIcon(":/icon/run.png"),tr("新建分类"));
    popMenu->addSeparator();
    QAction *movetoAction = popMenu->addAction(QIcon(":/icon/run.png"),tr("移动"));
    QAction *renameAction = popMenu->addAction(QIcon(":/icon/run.png"),tr("重命名"));
    QAction *deleteAction = popMenu->addAction(QIcon(":/icon/run.png"),tr("删除"));
    popMenu->addSeparator();
    QAction *refreshAction = popMenu->addAction(QIcon(":/icon/refresh.png"),tr("刷新"));

    QModelIndex index = this->currentIndex();
    QModelIndex indexChild = this->currentIndex().child(0, 0);
    bool ok = this->model()->data(indexChild).toBool();
    if(!ok)//当前活动项在叶子节点上
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
        newTypeAction->setDisabled(true);
        movetoAction->setDisabled(true);
        renameAction->setDisabled(true);
        deleteAction->setDisabled(true);
    }

    connect(deleteAction, SIGNAL( triggered( ) ), this, SLOT( OnDeleteItemSlot( ) ));
    connect(newTypeAction, SIGNAL( triggered( ) ), this, SLOT( OnInsertItemSlot( ) ));
    connect(refreshAction, SIGNAL( triggered( ) ), this, SLOT( OnRefreshItemSlot( ) ));

    popMenu->exec(cursor().pos());
}

void ScriptTreeView::load(const QString &filename)
{
    QFile modelFile(filename);
    if(!modelFile.exists())
    {
        QMessageBox::information(this->window(), tr("错误"), tr("数据文件丢失，请检查。"), QMessageBox::Ok);
        return;
    }
    if( !modelFile.open(QIODevice::ReadOnly) )
    {
        QMessageBox::information(this->window(), tr("错误"), tr("没有权限读取数据文件。"), QMessageBox::Ok);
        return;
    }
    QTextStream stream(modelFile.readAll(),QIODevice::ReadOnly);
    stream.setCodec( QTextCodec::codecForName("SYSTEM") );
    QByteArray byteArray;
    byteArray.append(stream.readAll());
    modelFile.close();

    QStringList headers;
    headers << tr("分类") << tr("数量");
    m_model= new TreeModel(headers, byteArray);
    this->setModel(m_model);
    setColumnHidden(1, true);
    restoreState();
}

void ScriptTreeView::save(const QString &filename)
{
    QString itemText;
    QByteArray byteArr;

    foreach (QModelIndex index, m_model->getPersistentIndexList())
    {
            itemText =  index.data(Qt::DisplayRole).toString();
            byteArr.append(itemText);
    }
    QFile modelFile(filename);
    modelFile.write(byteArr);
}

//call in thread
void ScriptTreeView::loadTree(const QByteArray &treeArray)
{
    QStringList headers;
    headers << tr("分类") << tr("数量");
    m_model = new TreeModel(headers, treeArray);
    this->setModel(m_model);
    setColumnHidden(1, true);
    restoreState();
}

void ScriptTreeView::saveState()
{
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
    settings.beginGroup("ScriptTree");
    //set to other chars
    settings.setIniCodec( QTextCodec::codecForName("SYSTEM") );
    settings.setValue("ExpandedItems", QVariant::fromValue(List));
    settings.endGroup();
}

void ScriptTreeView::restoreState()
{
    QStringList List;
    // get list
    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.beginGroup("ScriptTree");
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

void ScriptTreeView::OnDeleteItemSlot()
{
    QModelIndex index = this->selectionModel()->currentIndex();
    if( index.isValid())
    {
        //m_model = (TreeModel*)this->model();
        int currentRow = index.row();
        const QModelIndex parent = index.parent();
        qDebug() << currentRow << parent.data().toString();
        if ( m_model->removeRow(currentRow, parent) )
        {
            qDebug() << "delete successful.";
        }
        else
        {
            qDebug() << "delete failed.";
        }
    }
}

void ScriptTreeView::OnInsertItemSlot()
{
    QModelIndex index = this->selectionModel()->currentIndex();
    QAbstractItemModel *model = this->model();

    if (!model->insertRow(0, index))
        return;

    qDebug() << "insert failed.";
    //for (int column = 0; column < model->columnCount(index); ++column)
    {
        QModelIndex child = model->index(0, 0, index);
        model->setData(child, QVariant("[No data]"), Qt::EditRole);
        if (!model->headerData(0, Qt::Horizontal).isValid())
            model->setHeaderData(0, Qt::Horizontal, QVariant("[No header]"),Qt::EditRole);
    }
}

void ScriptTreeView::OnRefreshItemSlot()
{
     load(m_scriptFile);
}
