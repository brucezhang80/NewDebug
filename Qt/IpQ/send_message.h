#ifndef SEND_MESSAGE_H
#define SEND_MESSAGE_H
#include "message_base.h"

class SendMessage : public MessageBase
{
public:
    SendMessage();
    virtual ~SendMessage();

    SendMessage(QHostAddress address, quint16 port, QString additionalInfo,
                QString extendedInfo, qint32 flags);

    virtual SendMessage* clone() const;

    void setState(States state);
    MessageBase::States state() const;

    int sendTimes() const;
    void incrementSendTimes();


private:
    States      m_state;
    int         m_sendTimes;
};

#endif // SEND_MESSAGE_H
