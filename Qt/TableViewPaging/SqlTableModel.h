#ifndef SQLTABLEMODEL_H
#define SQLTABLEMODEL_H

#include <QSqlTableModel>

class SqlTableModel : public QSqlTableModel
{
    Q_OBJECT
public:
    explicit SqlTableModel(QObject *parent = 0, QSqlDatabase db = QSqlDatabase());

    void    setId(qint64 id){ m_id = id; }
    qint64  id(){return m_id; }
    void    setAPageCount(qint64 count){ m_nAPageCount = count; }
    qint64  aPageCount(){return m_nAPageCount; }
    void    setCurrentPage(qint64 nPage) {m_nCurPageCount = nPage;}
    qint64  currentPage(){ return m_nCurPageCount;}
//    qint64  totalPage(){ return m_nTotalPage; }
    qint64  total(){ return m_nTotal = this->rowCount();}
    void    updateModel();

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    int rowCount( const QModelIndex & parent = QModelIndex() );

signals:

public slots:
    void previousPage();
    void nextPage();
    void gotoPage(int index);

private:
    qint64          m_id;
    qint64          m_nStartId;         // ÿҳ��ʼid
    qint64          m_nAPageCount;      // ÿҳ����
    qint64          m_nCurPageCount;    // ��ǰҳ����
//    qint64          m_nTotalPage;     // ��ҳ��
    qint64          m_nTotal;           // �ܼ�¼��
};

#endif // SQLTABLEMODEL_H
