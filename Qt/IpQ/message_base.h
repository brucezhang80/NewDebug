#ifndef MESSAGE_BASE_H
#define MESSAGE_BASE_H

#include <QHostAddress>

#include "owner.h"

class MessageBase
{
public:
    MessageBase();
    virtual ~MessageBase();

    // Create receive message from receive packet
    MessageBase(QString packet, QHostAddress address, quint16 port);
    // Create send message
    MessageBase(QHostAddress address, quint16 port, QString additionalInfo,
            QString extendedInfo, quint32 flags);

    enum States
    {
        NotSend = 0,
        Sending,
        SendOk,
        SendFail,
        SendAckOk
    };

    virtual MessageBase* clone() const { return new MessageBase(*this); }

    virtual QString packet() const { return m_packet; }

    virtual QString packetNoString() const { return m_packetNoString; }

    virtual quint32 flags() const { return m_flags; }

    virtual quint16 port() const { return m_port; }

    virtual const Owner& owner() const { return m_owner; }

    virtual QString ip() const { return m_ipAddress.toString(); }
    virtual QHostAddress ipAddress() const { return m_ipAddress; }

    virtual QString additionalInfo() const { return m_additionalInfo; }
    virtual QString extendedInfo() const { return m_extendedInfo; }

    // NOTE: only send msg use these functions
    virtual void setState(States /*state*/) {}
    virtual MessageBase::States state() const { return MessageBase::NotSend;}
    virtual int sendTimes() const {return 1;}
    virtual void incrementSendTimes(){}

private:
    void parsePacket();         // 解析数据包
    void parseAdditionalInfo();
    void constructPacket();

    Owner       m_owner;
    QString     m_packet;
    QString     m_extendedInfo;
    QString     m_additionalInfo;
    QString     m_packetNoString;
    quint32     m_flags;
    QString     m_name;
    QHostAddress m_ipAddress;
    quint16     m_port;
};

#endif // MESSAGE_BASE_H
