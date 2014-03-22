#include "send_message.h"

SendMessage::SendMessage()
{
}

SendMessage::~SendMessage()
{
}

SendMessage::SendMessage(QHostAddress address, quint16 port, QString additionalInfo,
                     QString extendedInfo, qint32 flags)
    :MessageBase(address, port, additionalInfo, extendedInfo, flags),
      m_state(NotSend),
      m_sendTimes(0)
{

}

SendMessage *SendMessage::clone() const
{
    return new SendMessage(*this);
}

void SendMessage::setState(MessageBase::States state)
{
    m_state = state;
}

MessageBase::States SendMessage::state() const
{
    return m_state;
}

int SendMessage::sendTimes() const
{
    return m_sendTimes;
}

void SendMessage::incrementSendTimes()
{
    ++m_sendTimes;
}
