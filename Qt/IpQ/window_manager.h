#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include <QObject>
#include <QList>
#include <QMutex>
#include <QMap>

#include "message.h"

class QTreeWidgetItem;

class Owner;
class MessageWindow;
class MessageBox;
class UserItemWidget;

class WindowManager : public QObject
{
    Q_OBJECT
public:
    explicit WindowManager(QObject *parent = 0);
    ~WindowManager();
    
    void visibleMessageWindow();
    void visibleAllMessageWindow();
    void removeMessageWindow(MessageWindow* win);
    void setMessageWindowTheme(const QString &themeFile);

signals:
    
private slots:
    void newMessage(Message msg);
    void showMsgWindow(UserItemWidget*);
    void showMsgWindow(const QString &ip);
    void removeMessageWindow();
    void sendFileMessageBox(Message msg);

private:
    void recvMessageWindow(Message msg);
    bool createMessageReadedWindow(Message msg);
    void destroyMessageWindowMap();
    void createMessageBox(Message msg);
    bool isVisiibleMsgWindow(Message msg);

    QMap<QString, MessageWindow*>   m_messageWindowMap;
};

#endif // WINDOW_MANAGER_H
