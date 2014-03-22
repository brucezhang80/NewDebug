
#include "treeitem.h"
#include "sqltreemodel.h"
#include <QtCore/QTextStream>

SqlTreeModel::SqlTreeModel(QObject *parent) :
    m_rootItem(NULL),
    QAbstractItemModel(parent)
{
    m_groupColumns << 0;
    m_groupTitleFormat = "{0}";
}

SqlTreeModel::~SqlTreeModel()
{
    if(m_rootItem != NULL)
        delete m_rootItem;
}

void SqlTreeModel::setQuery(const QSqlQuery &query)
{
    m_query = query;
}

bool SqlTreeModel::select()
{
    if (!m_query.isActive())
            return false;

    QSqlRecord      record       = m_query.record();
    const int       record_count = record.count();
    QList<QVariant> root_data;

    for (int i = 0; i < record_count; ++i)
    {
        if (m_groupColumns.indexOf(i) == -1)
            root_data << record.fieldName(i);
    }

    m_rootItem = new TreeItem(root_data);
    create(m_rootItem);

    return true;
}

void SqlTreeModel::setGroupByIndexs(const QList<int> &columnIndexs)
{
    m_groupColumns = columnIndexs;
}

void SqlTreeModel::SetGroupTitleFormat(const QString &format)
{
    m_groupTitleFormat = format;
}

QVariant SqlTreeModel::getData(const QModelIndex &index, int role) const
{
    return data(index, role);
}

QVariant SqlTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    TreeItem* item = static_cast<TreeItem*>(index.internalPointer());
    return item->getData(index.column());
}

Qt::ItemFlags SqlTreeModel::getFlags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant SqlTreeModel::getHeaderData(int section, Qt::Orientation orientation, int role) const
{
    return headerData(section, orientation, role);
}

QVariant SqlTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return m_rootItem->getData(section);

    return QVariant();
}

QModelIndex SqlTreeModel::getIndex(int rowIndex, int columnIndex, const QModelIndex &parent) const
{
    return index(rowIndex, columnIndex, parent);
}

QModelIndex SqlTreeModel::index(int rowIndex, int columnIndex, const QModelIndex &parent) const
{
    if (!hasIndex(rowIndex, columnIndex, parent))
        return QModelIndex();

    TreeItem *parent_item;

    if (!parent.isValid())
        parent_item = m_rootItem;
    else
        parent_item = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *child_item = parent_item->getChild(rowIndex);

    if (child_item)
        return createIndex(rowIndex, columnIndex, child_item);
    else
        return QModelIndex();
}
QModelIndex SqlTreeModel::getParent(const QModelIndex &index) const
{
    return parent(index);
}

QModelIndex SqlTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem* child_item  = static_cast<TreeItem*>(index.internalPointer());
    TreeItem* parent_item = child_item->getParent();

    if (parent_item == m_rootItem)
        return QModelIndex();

    return createIndex(parent_item->getRowIndex(), 0, parent_item);
}

int SqlTreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem* parent_item;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parent_item = m_rootItem;
    else
        parent_item = static_cast<TreeItem*>(parent.internalPointer());

    return parent_item->getChildCount();
}

int SqlTreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->getColumnCount();
    else
        return m_rootItem->getColumnCount();
}

void SqlTreeModel::create(TreeItem *parent)
{
    TreeItem* current_parent = parent;

    while (m_query.next())
    {
        QString   group_title         = m_groupTitleFormat;
        const int group_columns_count = m_groupColumns.count();
        const int number_of_columns   = m_query.record().count();

        for (int i = 0; i < group_columns_count; ++i)
        {
            const int column_index = m_groupColumns.at(i);

            QString     column_arg;
            QTextStream arg_stream(&column_arg);
            arg_stream << "{" << column_index << "}";

            group_title.replace(column_arg, m_query.value(column_index).toString());
        }

        if (current_parent->getData(0).toString() != group_title)
        {
            QList<QVariant> parent_data;
            parent_data << group_title;

            for (int i = 1; i < number_of_columns; ++i)
            parent_data << "";

            TreeItem* parent_item = new TreeItem(parent_data, parent);
            parent->appendChild(parent_item);
            current_parent = parent_item;

            QList<QVariant> child_data;

            for (int j = 0; j < number_of_columns; ++j)
            {
                if (m_groupColumns.indexOf(j) == -1)
                    child_data << m_query.record().value(j);
            }

            TreeItem* child_item = new TreeItem(child_data, current_parent);
            current_parent->appendChild(child_item);
        }
        else
        {
            QList<QVariant> child_data;

            for (int i = 0; i < number_of_columns; ++i)
            {
                if (m_groupColumns.indexOf(i) == -1)
                    child_data << m_query.record().value(i);
            }

            TreeItem* child_item = new TreeItem(child_data, current_parent);
            current_parent->appendChild(child_item);
        }
    }//while
}

//end cpp
