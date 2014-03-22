#ifndef RECV_MESSAGE_H
#define RECV_MESSAGE_H
#include "message_base.h"

class RecvMessage : public MessageBase
{
public:
    RecvMessage();
    RecvMessage(QString packet, QHostAddress address, quint16 port)
        : MessageBase(packet, address, port) {}
    virtual ~RecvMessage() {}

    virtual RecvMessage* clone() const { return new RecvMessage(*this); }

};

#endif // RECV_MESSAGE_H
