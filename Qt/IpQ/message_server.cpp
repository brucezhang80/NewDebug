#include "macai.h"
#include "message_server.h"
#include "ipq.h"
#include "constants.h"
#include "recv_message.h"
#include "send_message.h"
#include "user_manager.h"
#include "message_thread.h"

#include <QMutexLocker>
#include <QTextCodec>
#include <QSet>
#include <QNetworkInterface>
#include <QAbstractSocket>

#include <QDebug>

MessageServer::MessageServer(QObject *parent) :
    QObject(parent)
{
    updateAddresses();
    connect(&m_udpSocket, SIGNAL(readyRead()), this, SLOT(readPacket()));
//    connect(this, SIGNAL(releaseFile(QString)),
//            Macai::sendFileManager, SLOT(removeTransferLocked(QString)));
    connect(&m_udpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(socketEerror(QAbstractSocket::SocketError)));
}

void MessageServer::start()
{
    m_udpSocket.bind( QHostAddress::Any, IPQ_DEFAULTPORT, QUdpSocket::ShareAddress
                      | QUdpSocket::ReuseAddressHint );
}

void MessageServer::readPacket()
{
    qDebug() << "readyRead for use to read packet.";

    while( m_udpSocket.hasPendingDatagrams() )
    {
        QHostAddress senderIp;
        quint16 senderPort;
        QByteArray datagram;
        datagram.resize(m_udpSocket.pendingDatagramSize());
        if (m_udpSocket.readDatagram(datagram.data(), datagram.size(),
                                     &senderIp, &senderPort) == -1)
        {
            continue;
        }

//        if (!isSupportedCommand(datagram))
//            continue;

        QTextCodec* codec = QTextCodec::codecForName("SYSTEM");
        QString packet = codec->toUnicode(datagram);
        //QString packet = Global::transferCodec->codec()->toUnicode(datagram);

        // 构造一个消息数据包
        RecvMessage recvMsg(packet, senderIp, senderPort);
        processRecvMsg(Message(recvMsg));

    }
}

void MessageServer::handleMsg(Message &msg)
{
    // Delete msg
    if (msg->state() == MessageBase::SendAckOk)
    {
        Macai::messageThread->removeSendMsgNotLock(msg->packetNoString());
    }

    // Delete msg
    if (msg->sendTimes() >= MaxResendTimes)
    {
        Macai::messageThread->removeSendMsgNotLock(msg->packetNoString());
    }
    if (msg->state() == MessageBase::NotSend || isResendNeeded(msg))
    {
        qDebug() << "MessageBase::NotSend";

        msg->setState(MessageBase::Sending);
        msg->incrementSendTimes();
        switch ( msg->flags() & MODE )
        {
        case IPQ_BROADCAST_ENTRY:
        case IPQ_BROADCAST_EXIT:
        case IPQ_BROADCAST_ABSENCE:
            broadcastUserMsg(msg);
            Macai::messageThread->removeSendMsgNotLock(msg->packetNoString());
            break;

        case IPQ_BROADCAST_ANSENTRY:
        case IPQ_ANSREAD_MESSAGE:
        case IPQ_RELEASE_FILES:
            broadcastUserMsg(msg);
            Macai::messageThread->removeSendMsgNotLock(msg->packetNoString());
            break;

        case IPQ_READ_MESSAGE:
            broadcastMsg(msg);
            qDebug() << "sucessfully sended message.";
            break;

        case IPQ_RECV_MESSAGE:
            broadcastMsg(msg);
            qDebug() << "sucessfully recived message.";
            break;

        case IPQ_SEND_MESSAGE:
        case IPQ_SEND_FILE:
            broadcastMsg(msg);
            break;

        default:
            Macai::messageThread->removeSendMsgNotLock(msg->packetNoString());
            break;
        }
    }

}

bool MessageServer::isSupportedCommand(QByteArray &/*datagram*/) const
{
    return false;
}

void MessageServer::broadcastMsg(Message &msg)
{
    qDebug() << "MessageServer::broadcastMsg";
    //QByteArray datagram = Global::transferCodec->codec()->fromUnicode(msg->packet());
    QTextCodec* codec = QTextCodec::codecForName("SYSTEM");
    QByteArray datagram = codec->fromUnicode(msg->packet());

    if (m_udpSocket.writeDatagram(datagram, msg->ipAddress(), msg->port()) == -1)
    {
        msg->setState(MessageBase::SendFail);
    }
    else
    {
        // Send Ok
        msg->setState(MessageBase::SendOk);
    }
}

void MessageServer::broadcastUserMsg(Message &msg)
{
    qDebug() << "MsgServer::broadcastUserMsg";

    updateAddresses();

    //QByteArray datagram = Global::transferCodec->codec()->fromUnicode(msg->packet());
    QTextCodec* codec = QTextCodec::codecForName("SYSTEM");
    QByteArray datagram = codec->fromUnicode(msg->packet());

    bool validBroadcastAddresses = true;
    foreach (QHostAddress address, m_broadcastAddresses)
    {
        if (m_udpSocket.writeDatagram(datagram, address, IPQ_DEFAULTPORT) == -1)
        {
            validBroadcastAddresses = false;
        }
    }

    if (!validBroadcastAddresses)
    {
        updateAddresses();
    }
}

bool MessageServer::isResendNeeded(Message &msg) const
{
    if (msg->state() == MessageBase::SendFail && msg->sendTimes() < MaxResendTimes)
        return true;

    return false;
}

void MessageServer::updateAddresses()
{
    m_broadcastAddresses.clear();
    m_ipAddresses.clear();

    foreach (QNetworkInterface interface, QNetworkInterface::allInterfaces())
    {
        foreach (QNetworkAddressEntry entry, interface.addressEntries())
        {
            QHostAddress broadcastAddress = entry.broadcast();
            if (broadcastAddress != QHostAddress::Null && entry.ip() != QHostAddress::LocalHost)
            {
                m_broadcastAddresses << broadcastAddress;
                m_ipAddresses << entry.ip();
            }
        }
    }

    // add 指定用户的广播地址
//    foreach(QString s, Global::preferences->userSpecifiedBroadcastIpList)
//    {
//        QHostAddress h(s);
//        if (h != QHostAddress::Null &&
//            h != QHostAddress::LocalHost &&
//            h != QHostAddress::LocalHostIPv6)
//        {
//            m_broadcastAddresses << h;
//        }
//    }

    // 只是去除重复的
    QSet<QHostAddress> set = m_broadcastAddresses.toSet();
    m_broadcastAddresses.clear();
    m_broadcastAddresses = set.toList();
}


void MessageServer::processRecvMsg(Message msg)
{
    qDebug() << "socket ready for use then prosess recieve message.";
    switch( msg->flags() & MODE)
    {
    case IPQ_BROADCAST_ENTRY:
        processEntryMsg(msg);
        break;

    case IPQ_BROADCAST_EXIT:
        qDebug() << "exit";
        emit newExitMsg(msg);
        break;

    case IPQ_BROADCAST_ANSENTRY:
        emit newUserMsg(msg);
        break;

    case IPQ_BROADCAST_ABSENCE:
        break;

    case IPQ_SEND_MESSAGE:
    case IPQ_SEND_FILE:
        processRecvSendMsg(msg);
        break;

    case IPQ_READ_MESSAGE:
        processRecvReadMsg(msg);
        break;

    case IPQ_ANSREAD_MESSAGE:
    case IPQ_RECV_MESSAGE:
    case IPQ_DEL_MESSAGE:
        break;

    case IPQ_RELEASE_FILES:
        processRecvReleaseFilesMsg(msg);
        break;

    default:
        break;
    }

}

void MessageServer::processRecvReleaseFilesMsg(Message msg)
{
    qDebug() << "processRecvReleaseFilesMsg";
}

void MessageServer::processRecvReadMsg(Message msg)
{
    qDebug() << "processRecvReadMsg";
    emit newMessage(msg);
    SendMessage sendMsg(msg->ipAddress(), msg->port(), msg->packetNoString(),
                    ""/* extendedInfo */, IPQ_ANSREAD_MESSAGE);

    Macai::messageThread->addSendMsg(Message(sendMsg));
}

void MessageServer::processRecvSendMsg(Message msg)
{
    qDebug() << "processRecvSendMsg";

    if ( msg->flags() & OPTION)
    {
        SendMessage sendMsg(msg->ipAddress(), msg->port(), msg->packetNoString(),
                ""/* extendedInfo */, IPQ_RECV_MESSAGE);

        Macai::messageThread->addSendMsg(Message(sendMsg));
    }

    // If sender is not in our user list, add it.
    if (!Macai::userManager->contains(msg->ip()))
    {
        emit newUserMsg(msg);
    }

    emit newMessage(msg);
}

void MessageServer::processEntryMsg(Message msg)
{
    qDebug() << "while prosess recieve message: 有新用户登录: "
             << "Name:" << msg->owner().name()
             << "IP:" <<  msg->ipAddress()
             << "Port:" << msg->port(); ;
    emit newUserMsg(msg);

    SendMessage sendMsg(msg->ipAddress(), msg->port(),
                    Macai::userManager->entryMessage(),
                    ""/* extendedInfo */, IPQ_BROADCAST_ANSENTRY);

    Macai::messageThread->addSendMsg(Message(sendMsg));
}

// 发送广播消息
void MessageServer::processSendMsg()
{
    // We are a friend of MsgThread class
    QMutexLocker locker(&Macai::messageThread->m_lock);
//    qDebug() << "BINDING...";
    foreach (Message msg, Macai::messageThread->m_sendMsgMap)
    {
        // 处理发送的消息数据包
        handleMsg(msg);
    }
}

void MessageServer::socketEerror(QAbstractSocket::SocketError errorCode)
{
    emit error(errorCode, m_udpSocket.errorString());
}
