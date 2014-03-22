#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

class TreeItem;

class PicProjectViewModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    PicProjectViewModel(const QStringList &headers, const QStringList &data, QObject *parent = 0);
    ~PicProjectViewModel();

    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole);

    bool insertColumns(int position, int columns, const QModelIndex &parent = QModelIndex());
    bool removeColumns(int position, int columns, const QModelIndex &parent = QModelIndex());
    bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex());

    QModelIndexList getPersistentIndexList()
    {
        return QAbstractItemModel::persistentIndexList();
    }
    void reset()
    {
        QAbstractItemModel::reset();
    }
    //添加新下载项目:
    void setupModelData(const QStringList &datas/*, TreeItem *parent*/);

private:

    TreeItem *getItem(const QModelIndex &index) const;

    TreeItem *rootItem;
    
};

#endif // TREEMODEL_H
