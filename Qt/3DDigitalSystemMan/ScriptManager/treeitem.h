#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
#include <QVariant>
#include <QVector>

 class TreeItem
 {
 public:
     TreeItem(const QVector<QVariant> &data, TreeItem *parent = 0);
     ~TreeItem();

     //void appendChild(TreeItem *child);

     TreeItem *child(int row);
     int childCount() const;
     int columnCount() const;
     QVariant data(int column) const;
     int row() const; // == childNumber()
     TreeItem *parent();

    bool insertChildren(int position, int count, int columns);
    bool insertColumns(int position, int columns);
    bool removeChildren(int position, int count);
    bool removeColumns(int position, int columns);
    bool setData(int column, const QVariant &value);

 private:
     QList<TreeItem*> childItems;
     QVector<QVariant> itemData;
     TreeItem *parentItem;
 };
#endif // TREEITEM_H
