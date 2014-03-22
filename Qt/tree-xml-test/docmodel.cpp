#include "docmodel.h"
#include "docitem.h"

#include <QStringList>
#include <QDebug>

DocModel::DocModel(QObject *parent) :
    QAbstractItemModel(parent)
{
}

DocModel::DocModel(QDomDocument document, QObject *parent):
    QAbstractItemModel(parent), m_domDocument(document)
{
    m_rootItem = new DocItem(m_domDocument, 0);
}

DocModel::~DocModel()
{
    delete m_rootItem;
}

QVariant DocModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    DocItem *item = static_cast<DocItem*>(index.internalPointer());

    return item->data(0);
}

Qt::ItemFlags DocModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QVariant DocModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section);
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 0)
        return tr("Name");


    return QVariant();
}

QModelIndex DocModel::index(int row, int column, const QModelIndex &parent) const
{
    if ( !hasIndex(row, column, parent) )
        return QModelIndex();

    DocItem *parentItem;

    if ( !parent.isValid() )
        parentItem = m_rootItem;
    else
        parentItem = static_cast<DocItem*>(parent.internalPointer());

    DocItem *childItem = parentItem->child(row);
    if ( !childItem )
        return QModelIndex();

    return createIndex(row, column, childItem);
}

QModelIndex DocModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    DocItem *childItem = static_cast<DocItem*>(child.internalPointer());

    DocItem *parentItem = childItem->parent();

    if (!parentItem || parentItem == m_rootItem)
    {
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

int DocModel::rowCount(const QModelIndex &parent) const
{
//    if (parent.column() > 0)
//        return 0;

    DocItem *parentItem;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<DocItem*>(parent.internalPointer());

    return parentItem->node().childNodes().count();
}

int DocModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return 1;
}
