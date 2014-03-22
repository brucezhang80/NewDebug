#ifndef SEND_HISTORY_TABLEVIEW_H
#define SEND_HISTORY_TABLEVIEW_H

#include <QTableView>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QList>
#include <QContextMenuEvent>

class QMenu;

class SendHistoryTableView : public QTableView
{
    Q_OBJECT
public:
    explicit SendHistoryTableView(QWidget *parent = 0);
    ~SendHistoryTableView();
    void retranslateUi();

signals:
    
private slots:
    void updateHistoryLogs(const QList<QString> &logs);
    void openFile();
    void deleteFile();
    void refresh();

private:
    bool createConnectionDB();
    void initializeModel(QSqlTableModel *model);
    void insertTable(const QList<QString> &logs);
    void setConnect();

    QSqlTableModel      *m_model;
    QSqlDatabase        m_db;
    QString             m_dbName;

    QAction             *m_openAct;
    QAction             *m_deleteAct;
    QAction             *m_refreshAct;

protected:
    void contextMenuEvent(QContextMenuEvent *event);
};

#endif // SEND_HISTORY_TABLEVIEW_H
