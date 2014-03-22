#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QTableWidget>
#include <QStandardItemModel>

#include "personclient.h"


class PersonManagerView : public QTableWidget
{
    Q_OBJECT
public:
    explicit PersonManagerView(QWidget *parent = 0);
    void initTableView();
    QString         m_myUserName;
    QString         m_myHostName;
    QString         m_myHostAddress;
    QString         m_myHostPort;

signals:
    
public slots:
    void newParticipantSlot(const QString&, const QString&, const QString&);
    void participantLeftSlot(const QString&, const QString&, const QString&);
    void addMyself();

private:
    PersonClient    *m_personClient;

    QModelIndex     m_widgetModelIndex;
};

#endif // TABLEVIEW_H
