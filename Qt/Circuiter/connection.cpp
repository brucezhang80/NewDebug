#include "connection.h"
#include <QtNetwork>

#include <QDebug>

static const int TransferTimeout = 30 * 1000;
static const char SeparatorToken = ' ';

Connection::Connection(QObject *parent) :
    QTcpSocket(parent)
{
    m_connectedMessage = tr("Unknow");
    m_userName = tr("Unknown Name");
//    m_localHostName = localHostName();
//    m_localHostAddress = peerAddress().toString();
//    m_localHostPort = QString::number(peerPort());

    QObject::connect(this, SIGNAL(readyRead()), this, SLOT(processReadyRead()));
    QObject::connect(this, SIGNAL(disconnected()), this, SLOT(stop()));
    connect(this, SIGNAL(connected()), this, SLOT(connected()));

}

QString Connection::userName() const
{
    return m_userName;
}

QString Connection::localHostName() const
{
    return m_localHostName;
}

QString Connection::localHostAddress() const
{
    return m_localHostAddress;
}

QString Connection::localHostPort() const
{
    return m_localHostPort;
}

void Connection::connected()
{
    QByteArray online = m_connectedMessage.toUtf8();
    QByteArray data = "Online " + QByteArray::number(online.size()) + ' ' + online;
    if (write(data) == data.size())
        m_isOnline = true;
}

void Connection::processReadyRead()
{
    qDebug() << "processReadyRead .";
    m_localHostName = this->peerName();// QString(m_buffer);
    m_localHostAddress = peerAddress().toString();
    m_localHostPort = QString::number(peerPort());

    qDebug() << "you have connected: " << m_localHostName << m_localHostAddress << m_localHostPort;

//    if (!hasEnoughData())
//        return;

//    m_buffer = read(numBytesForCurrentDataType);
//    if (m_buffer.size() != numBytesForCurrentDataType)
//    {
//        abort();
//        return;
//    }

//    if (!isValid())
//    {
//        abort();
//        return;
//    }
//    emit readyForUse();
//    do
//    {
//        if (!readProtocolHeader())
//            return;
//        if (!hasEnoughData())
//            return;
//        processData();
//    } while (bytesAvailable() > 0);
}

void Connection::stop()
{
    qDebug() << "you have stop .";
}

////////////////////////////////////////////////////////////////////
int Connection::readDataIntoBuffer(int maxSize /*= MaxBufferSize*/)
{
    if (maxSize > MaxBufferSize)
        return 0;

    int numBytesBeforeRead = m_buffer.size();
    if (numBytesBeforeRead == MaxBufferSize)
    {
        abort();
        return 0;
    }

    while (bytesAvailable() > 0 && m_buffer.size() < maxSize)
    {
        m_buffer.append(read(1));
        if (m_buffer.endsWith(SeparatorToken))
            break;
    }

    return m_buffer.size() - numBytesBeforeRead;
}

int Connection::dataLengthForCurrentDataType()
{
    if (bytesAvailable() <= 0 || readDataIntoBuffer() <= 0 || !m_buffer.endsWith(SeparatorToken))
        return 0;

    m_buffer.chop(1);
    int number = m_buffer.toInt();
    m_buffer.clear();

    return number;
}


bool Connection::hasEnoughData()
{
    if (numBytesForCurrentDataType <= 0)
        numBytesForCurrentDataType = dataLengthForCurrentDataType();

    if (bytesAvailable() < numBytesForCurrentDataType || numBytesForCurrentDataType <= 0)
    {
        return false;
    }

    return true;
}
