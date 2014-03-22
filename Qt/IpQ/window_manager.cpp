#include "macai.h"
#include "ipq.h"
#include "constants.h"
#include "message_thread.h"
#include "system.h"
#include "preferences.h"
#include "owner.h"
#include "message_window.h"
#include "window_manager.h"
#include "usertreewidget.h"
#include "useritemwidget.h"
#include "main_window.h"
#include "send_message.h"
#include "message_box.h"
#include "transfer_file_window.h"

#include <QPoint>
#include <QSize>
#include <QTreeWidgetItem>
#include <QDebug>

WindowManager::WindowManager(QObject *parent) :
    QObject(parent)
{
    connect(Macai::userTreeWidget, SIGNAL(showMsgWindow(UserItemWidget*)),
            this, SLOT(showMsgWindow(UserItemWidget*)) );
}

WindowManager::~WindowManager()
{
//    destroyMessageWindowMap();
}

void WindowManager::visibleMessageWindow()
{
//    foreach (MessageWindow *w, m_messageWindowList)
//    {
//        if (!w->isVisible())
//        {
//            w->show();
//        }
//    }
}

void WindowManager::visibleAllMessageWindow()
{
//    foreach (MessageWindow *w, m_messageWindowList)
//    {
//        // 使窗口置顶显示
//        w->setWindowFlags(w->windowFlags() | Qt::WindowStaysOnTopHint);
//        QPoint pos = w->pos();
//        w->move(pos);
//        w->show();
//        w->setWindowFlags(w->windowFlags() & ~Qt::WindowStaysOnTopHint);
//        w->move(pos);
//        w->show();
//    }
}

// slot function
void WindowManager::newMessage(Message msg)
{
    qDebug() << "newMessage(Message msg)" << (msg->flags() & MODE);
    switch( msg->flags() & MODE)
    {
    case IPQ_SEND_MESSAGE:
        createMessageBox(msg);
        recvMessageWindow(msg);
        break;
    case IPQ_SEND_FILE:
        qDebug() << "send file.";
        sendFileMessageBox(msg);
        break;
    default:
        break;
    }
}

void WindowManager::showMsgWindow(UserItemWidget *userwidget)
{
    if( !m_messageWindowMap.contains(userwidget->userIp()) )
    {
        MessageWindow *win = new MessageWindow(userwidget->hostName(), userwidget->userIp(), userwidget->userGroup());
//        MessageWindow *win = new MessageWindow(sendMsg);
        connect(win, SIGNAL(removeMessageWindow()), this, SLOT(removeMessageWindow()) );
        m_messageWindowMap.insert(userwidget->userIp(), win);
        win->show();
    }
    else
    {
        MessageWindow *msgWin = m_messageWindowMap.value(userwidget->userIp());
        if( msgWin && msgWin->isWindow() )
        {
            msgWin->showNormal();
        }
    }
}

void WindowManager::showMsgWindow(const QString &ip)
{
    if( !m_messageWindowMap.contains(ip) )
    {
        MessageWindow *win = new MessageWindow("", ip, "");
        connect(win, SIGNAL(removeMessageWindow()), this, SLOT(removeMessageWindow()) );
        m_messageWindowMap.insert(ip, win);
        win->show();
    }
    else
    {
        MessageWindow *msgWin = m_messageWindowMap.value(ip);
        if( msgWin && msgWin->isWindow() )
        {
            msgWin->showNormal();
        }
    }
}

void WindowManager::recvMessageWindow(Message msg)
{
    if( !m_messageWindowMap.contains(msg->ip()) )
    {
        MessageWindow *win = new MessageWindow(msg);
        connect(win, SIGNAL(removeMessageWindow()), this, SLOT(removeMessageWindow()) );
        m_messageWindowMap.insert(msg->ip(), win);
    }

    MessageWindow *msgWin = m_messageWindowMap.value(msg->ip());
    if( msgWin && msgWin->isWindow() )
    {
        // *连续多次发送消息而不打开阅读,出现内存溢,导致程序crash
        msgWin->recvMessage(msg);
    }
#if 0
    // 有新消息播放声音
    if( !Macai::preferences->isNoSoundAlarm )
    {
        QString sound_file = Macai::preferences->noticeSound;
        // soundclass paly sound
        // ...
    }

    // 有新消息时自动显示消息窗口
    if( !Macai::preferences->isNoAutoPopupMsg)
    {
        msg_win->show();
    }
#endif
}

bool WindowManager::createMessageReadedWindow(Message msg)
{
    qDebug() << "recive:" << msg->additionalInfo() << msg->owner().name();

    return false;
}

void WindowManager::removeMessageWindow()
{
    MessageWindow* win = static_cast<MessageWindow*>(sender());
    if( win )
    {
        disconnect(win, SIGNAL(removeMessageWindow()), this, SLOT(removeMessageWindow()) );
        qDebug() << "removed successful:" << win->hostName() << win->ip();
//        delete m_messageWindowMap.value(win->ip());
        m_messageWindowMap.remove(win->ip());
    }
    qDebug() << m_messageWindowMap;

}

void WindowManager::setMessageWindowTheme(const QString &themeFile)
{
    if(m_messageWindowMap.count() == 0)
        return;
    QMap<QString, MessageWindow*>::iterator it = m_messageWindowMap.begin();
    while ( it != m_messageWindowMap.end() )
    {
        MessageWindow *win = it.value();
        win->setTheme(themeFile);
        ++it;
    }
}

void WindowManager::sendFileMessageBox(Message msg)
{
    Macai::messageBox->sendFileMessage(msg->owner().name(), msg->owner().ip());
    Macai::messageBox->show();
    Macai::messageBox->raise();
    Macai::messageBox->activateWindow();
}

void WindowManager::destroyMessageWindowMap()
{
    QMap<QString, MessageWindow*>::iterator it = m_messageWindowMap.begin();

    while ( it != m_messageWindowMap.end() )
    {
        delete it.value();
        ++it;
    }
    m_messageWindowMap.clear();
}

void WindowManager::createMessageBox(Message msg)
{
    bool ret = isVisiibleMsgWindow(msg);
    if(ret) return;

    Macai::messageBox->insertUserMsgItem(msg->owner().name(), msg->owner().ip());
    Macai::messageBox->show();
    Macai::messageBox->raise();
    Macai::messageBox->activateWindow();

    connect(Macai::messageBox, SIGNAL(readMessage(QString)), this, SLOT(showMsgWindow(QString)) );
}

bool WindowManager::isVisiibleMsgWindow(Message msg)
{
    if( m_messageWindowMap.contains(msg->ip()) )
    {
        MessageWindow *msgWin = m_messageWindowMap.value(msg->ip());
        if( msgWin && msgWin->isVisible() )
        {
            return true;
        }
    }

    return false;
}

