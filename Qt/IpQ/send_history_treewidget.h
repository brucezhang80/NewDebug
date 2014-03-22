#ifndef SEND_HISTORY_TREEWIDGET_H
#define SEND_HISTORY_TREEWIDGET_H

#include <QTreeWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlDriver>
#include <QSqlRecord>

class SendHistoryTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit SendHistoryTreeWidget(QWidget *parent = 0);
    int count();

signals:
    
public slots:
    void updateHistoryLogs(const QList<QString> &logs);

private:
    void initialize();
    void setConnect();
    bool connectDB();

    QSqlDatabase    m_db;
    
};

#endif // SEND_HISTORY_TREEWIDGET_H
