#ifndef FRIENDSTABLEWND_H
#define FRIENDSTABLEWND_H

#include <QTableWidget>

class UserWidget;

class FriendsTableWnd : public QTableWidget
{
    Q_OBJECT
public:
    explicit FriendsTableWnd(QWidget *parent = 0);
    void initTable();

signals:
    
public slots:
    void newParticipant(const QString&, const QString&, const QString&);
    void newParticipant(const QString&);
    void showPrivateChat(int, int);
};

#endif // FRIENDSTABLEWND_H
