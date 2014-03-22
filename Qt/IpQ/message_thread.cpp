#include "macai.h"
#include "message_thread.h"
#include "ipq.h"
#include "constants.h"
#include "message_server.h"
#include "user_manager.h"
#include "message.h"
#include "window_manager.h"

#include <QMessageBox>
#include <QTimer>
#include <QDebug>

int id = qRegisterMetaType<Message>("Message");

MessageThread::MessageThread(QObject *parent) :
    QThread(parent)
{
}

MessageThread::~MessageThread()
{
    m_messageServer->processSendMsg();

    exit(0);
    wait();
}

void MessageThread::run()
{
    QTimer timer;
    timer.setInterval( SendMsgProcessInterval );
    timer.start();

    m_messageServer = new MessageServer;

    connect(&timer, SIGNAL(timeout()), m_messageServer, SLOT(processSendMsg()));

    connect(m_messageServer, SIGNAL(newMessage(Message)),
            Macai::windowManager, SLOT(newMessage(Message)));
    connect(m_messageServer, SIGNAL(newUserMsg(Message)),
            Macai::userManager, SLOT(newUserMsg(Message)) );
    connect(m_messageServer, SIGNAL(newExitMsg(Message)), Macai::userManager, SLOT(newExitMsg(Message)));

    connect(m_messageServer, SIGNAL(error(QAbstractSocket::SocketError, QString)),
            this, SLOT(handleError(QAbstractSocket::SocketError, QString)));

    m_messageServer->start();

    exec();
}

void MessageThread::addSendMsg(Message msg)
{
    QMutexLocker locker(&m_lock);
    m_sendMsgMap.insert(msg->packetNoString(), msg);
}

void MessageThread::removeSendMsg(QString packetNoString)
{
    QMutexLocker locker(&m_lock);

    m_sendMsgMap.remove(packetNoString);

}

void MessageThread::removeSendMsgNotLock(QString packetNoString)
{
    m_sendMsgMap.remove(packetNoString);
}

//void MessageThread::usleep(unsigned long secs)
//{
//    usleep(secs);
//}

void MessageThread::handleError(QAbstractSocket::SocketError errorCode, QString s)
{
    qDebug() << "MsgThread::handleError";

    QString errorString(QObject::tr("udp server error"));

    QMessageBox::critical(0, QObject::tr("IPQ"), errorString + ":\n" + s + ".");

    qDebug() << "MsgThread::handleError:" << s;

    if (errorCode == QAbstractSocket::AddressInUseError)
        ::exit(-1);
}
