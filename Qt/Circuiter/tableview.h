#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QTableView>
#include <QStandardItemModel>

class TableView : public QTableView
{
    Q_OBJECT
public:
    explicit TableView(QWidget *parent = 0);
    void initTableView();
    void createHeader();

signals:
    
public slots:

private:
    QStandardItemModel  *m_model;
    
};

#endif // TABLEVIEW_H
