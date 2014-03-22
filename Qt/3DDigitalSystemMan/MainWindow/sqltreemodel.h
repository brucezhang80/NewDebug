#ifndef SQLTREEMODEL_H
#define SQLTREEMODEL_H


#include <QAbstractItemModel>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>

class TreeItem;

class SqlTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit SqlTreeModel(QObject *parent = 0);
    ~SqlTreeModel();

    void setQuery(const QSqlQuery& query);
    bool select();
    void setGroupByIndexs(const QList<int>& columnIndexs);
    void SetGroupTitleFormat(const QString& format);
    QVariant getData(const QModelIndex& index, int role) const;
    Qt::ItemFlags getFlags(const QModelIndex& index) const;
    QVariant getHeaderData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QModelIndex getIndex(int rowIndex, int columnIndex, const QModelIndex& parent = QModelIndex()) const;
    QModelIndex getParent(const QModelIndex& index) const;
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;

protected:
    QVariant data(const QModelIndex& index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QModelIndex index(int rowIndex, int columnIndex, const QModelIndex& parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex& index) const;

    void create(TreeItem* parent);

private:
    TreeItem*  m_rootItem;

    QSqlQuery  m_query;
    QList<int> m_groupColumns;
    QString    m_groupTitleFormat;



signals:
    
public slots:
    
};

#endif // SQLTREEMODEL_H
