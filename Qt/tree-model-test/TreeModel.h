#ifndef TREEMODEL_HPP
#define TREEMODEL_HPP

#include <QAbstractItemModel>
#include <QSqlQuery>
#include <QVariant>
#include <QVector>

class QueryItem;

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit TreeModel(QSqlQuery *sqlQuery);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;

    bool setData( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );
    bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());

public:
    void visitChildren(QModelIndex index, QList<qint64> &idList);
    bool romoveChildren(const QModelIndex &index);
    void setParentId(const int &sId, const int &pId);

    QueryItem       *m_queryItem;
};

#endif // TREEMODEL_HPP
