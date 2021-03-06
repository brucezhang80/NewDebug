#include <QtGui>
#include <QStringList>
#include "treeitem.h"
#include "treemodel.h"


PicProjectViewModel::PicProjectViewModel(const QStringList &headers, const QStringList &data,
                     QObject *parent): QAbstractItemModel(parent)
{
    QVector<QVariant> rootData;
    foreach (QString header, headers)
        rootData << header;

    rootItem = new TreeItem(rootData);
    //setupModelData(data, rootItem);
}

 PicProjectViewModel::~PicProjectViewModel()
 {
     delete rootItem;
 }

 int PicProjectViewModel::columnCount(const QModelIndex &parent) const
 {
     if (parent.isValid())
         return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
     else
         return rootItem->columnCount();
 }

 QVariant PicProjectViewModel::data(const QModelIndex &index, int role) const
 {
     if (!index.isValid())
         return QVariant();

     if (role != Qt::DisplayRole)
         return QVariant();

     TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

     return item->data(index.column());
 }

 Qt::ItemFlags PicProjectViewModel::flags(const QModelIndex &index) const
 {
     if (!index.isValid())
         return 0;

     return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
 }

 bool PicProjectViewModel::setData(const QModelIndex &index, const QVariant &value, int role)
 {
     if (role != Qt::EditRole)
              return false;

          TreeItem *item = getItem(index);
          bool result = item->setData(index.column(), value);

          if (result)
              emit dataChanged(index, index);

          return result;
 }

 bool PicProjectViewModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
 {
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = rootItem->setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
 }

 bool PicProjectViewModel::insertColumns(int position, int columns, const QModelIndex &parent)
 {
    bool success;

    beginInsertColumns(parent, position, position + columns - 1);
    success = rootItem->insertColumns(position, columns);
    endInsertColumns();

    return success;
 }

 bool PicProjectViewModel::removeColumns(int position, int columns, const QModelIndex &parent)
 {
    bool success;

    beginRemoveColumns(parent, position, position + columns - 1);
    success = rootItem->removeColumns(position, columns);
    endRemoveColumns();

    if (rootItem->columnCount() == 0)
        removeRows(0, rowCount());

    return success;
 }

 bool PicProjectViewModel::insertRows(int position, int rows, const QModelIndex &parent)
 {
    TreeItem *parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows, rootItem->columnCount());
    endInsertRows();

    return success;
 }

 bool PicProjectViewModel::removeRows(int position, int rows, const QModelIndex &parent)
 {
    TreeItem *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
 }

 TreeItem *PicProjectViewModel::getItem(const QModelIndex &index) const
 {
     if (index.isValid())
     {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (item) return item;
    }

    return rootItem;
 }

 QVariant PicProjectViewModel::headerData(int section, Qt::Orientation orientation, int role) const
 {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

        return QVariant();
 }

 QModelIndex PicProjectViewModel::index(int row, int column, const QModelIndex &parent)
             const
 {
     if (!hasIndex(row, column, parent))
         return QModelIndex();

     TreeItem *parentItem = getItem(parent);

     if (!parent.isValid())
         parentItem = rootItem;
     else
         parentItem = static_cast<TreeItem*>(parent.internalPointer());

     TreeItem *childItem = parentItem->child(row);
     if (childItem)
         return createIndex(row, column, childItem);
     else
         return QModelIndex();
 }

 QModelIndex PicProjectViewModel::parent(const QModelIndex &index) const
 {
     if (!index.isValid())
         return QModelIndex();

     TreeItem *childItem = getItem(index);
     TreeItem *parentItem = childItem->parent();

     if (parentItem == rootItem)
         return QModelIndex();

     return createIndex(parentItem->row(), 0, parentItem);//row() == childNumber()
 }

 int PicProjectViewModel::rowCount(const QModelIndex &parent) const
 {
     TreeItem *parentItem = getItem(parent);
     if (parent.column() > 0)
         return 0;

     if (!parent.isValid())
         parentItem = rootItem;
     else
         parentItem = static_cast<TreeItem*>(parent.internalPointer());

     return parentItem->childCount();
 }

 //这个需要重新设计
 void PicProjectViewModel::setupModelData(const QStringList &datas/*, TreeItem *parent*/)
 {
    QVector<QVariant> rootData;
    rootData << datas[0] << datas[1];
    //TreeItem *parent = new TreeItem(rootData);

    TreeItem* titleItem=NULL;
    //rootItem = new TreeItem(rootData);
    int column = 0;
    for(int i = 0; i < datas.count(); ++i)
    {
        qDebug() << datas[i];
        //if( i < 1)
        {
            rootItem->insertChildren(rootItem->childCount(), 1, i);
            titleItem = rootItem->child(rootItem->childCount()-1);
            titleItem->setData(1, datas[i]);
        }
        //else if(titleItem)
        {
            //titleItem->child(titleItem->childCount()-1)->setData(i, datas[i]);
        }
        //column++;
    }

     /*
     QList<TreeItem*> parents;
     QList<int> indentations;
     parents << parent;
     indentations << 0;

     int number = 0;

     while (number < lines.count())
     {
         int position = 0;
         while (position < lines[number].length())
         {
             if (lines[number].mid(position, 1) != " ")
                 break;
             position++;
         }

         QString lineData = lines[number].mid(position).trimmed();

         if (!lineData.isEmpty())
         {
             // Read the column data from the rest of the line.
             QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
             QList<QVariant> columnData;
             for (int column = 0; column < columnStrings.count(); ++column)
                 columnData << columnStrings[column];

             if (position > indentations.last())
             {
                 // The last child of the current parent is now the new parent
                 // unless the current parent has no children.

                 if (parents.last()->childCount() > 0)
                 {
                     parents << parents.last()->child(parents.last()->childCount()-1);
                     indentations << position;
                 }
             }
             else
             {
                 while (position < indentations.last() && parents.count() > 0)
                 {
                     parents.pop_back();
                     indentations.pop_back();
                 }
             }

             // Append a new item to the current parent's list of children.
            TreeItem *parent = parents.last();
            parent->insertChildren(parent->childCount(), 1, rootItem->columnCount());
            for (int column = 0; column < columnData.size(); ++column)
                parent->child(parent->childCount() - 1)->setData(column, columnData[column]);
         }

         number++;
     }
     */
 }


