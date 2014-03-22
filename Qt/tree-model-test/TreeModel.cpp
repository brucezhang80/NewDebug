#include <QVector>
#include <QMessageBox>
#include <QStringListModel>
#include <QMimeData>

#include <QDebug>

#include "TreeModel.h"
#include "queryitem.h"
#include "constance.h"

/**
 * 考虑是否需要将QSqlQuery封装一下?目的是操作数据库与tree model同步
 */

TreeModel::TreeModel(QSqlQuery *sqlQuery):
    QAbstractItemModel()
{
    m_queryItem = new QueryItem(sqlQuery);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    qint32 parentId;
    if (parent.isValid())
        parentId = parent.internalId();
    else
        parentId = 0;

    return m_queryItem->childCount(parentId);
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return 1;
}


QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || role != Qt::DisplayRole)
        return QVariant();

    return m_queryItem->data(index.internalId());
}


QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section);
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 0)
        return QVariant(QString("Tag"));

    return QVariant();
}


Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}


QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    qint32 parentId;
    if( parent.isValid() )
        parentId = parent.internalId();
    else
        parentId = 0;

    int nPtr;
    int ret = m_queryItem->index(row, parentId, nPtr).toInt();
    if( ret == 1 )
        return createIndex(row, column, nPtr);

    return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if( !index.isValid() )
        return QModelIndex();

    int row, parent;
    int ret = m_queryItem->parent(index.internalId(), row, parent).toInt();

    if( ret == 0 )
        return QModelIndex();

    return createIndex(row, 0, parent);
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if( !index.isValid() || role != Qt::EditRole)
        return false;

    if( value.toString().isEmpty() )
    {
        return false;
    }

    return m_queryItem->setData(index.internalId(), value);
}

bool TreeModel::insertRows( int position, int rows, const QModelIndex &parent)
{
    bool success = true;
    int subId = m_queryItem->lastId()+1 ;

    beginInsertRows(parent, position, position + rows - 1);

    success = m_queryItem->insertRecord(subId, parent.internalId());

    endInsertRows();

    return success;
}

bool TreeModel::removeRows(int position, int rows, const QModelIndex &index/* = QModelIndex()*/)
{
    bool success = true;

    beginRemoveRows(index, position, position + rows - 1);

    success = romoveChildren(index);

    endRemoveRows();

    return success;
}

// 遍历子节点所有的model索引id号并保存起来,用来同步(删除)数据库sID
void TreeModel::visitChildren(QModelIndex index, QList<qint64> &idList)
{
    QModelIndex ch = index.child(0, 0);
    int row = 0;
    while( ch.isValid() )
    {
        qint64 id = ch.internalId();
        idList.append(id);
        visitChildren(ch, idList);
        row++;
        ch = ch.sibling(row, 0);
    }

    idList.append(index.internalId());
}

bool TreeModel::romoveChildren(const QModelIndex &index)
{
    bool success = true;

    QList<qint64> ids;
    visitChildren(index, ids);
    foreach( qint64 id, ids)
    {
        bool ret = m_queryItem->removeRecord(id);
        if( !ret )
        {
            success = false;
            break;
        }
    }

    return success;
}

void TreeModel::setParentId(const int &sId, const int &pId)
{
    m_queryItem->setParentId(sId, pId);
}

