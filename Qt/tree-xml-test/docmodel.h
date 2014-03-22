#ifndef DOCMODEL_H
#define DOCMODEL_H

#include <QAbstractItemModel>
#include <QDomDocument>

class DocItem;

class DocModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit DocModel(QObject *parent = 0);
    explicit DocModel(QDomDocument document, QObject *parent = 0);
    ~DocModel();

    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

signals:
    
public slots:

private:
     QDomDocument   m_domDocument;
     DocItem        *m_rootItem;
};

#endif // DOCMODEL_H
