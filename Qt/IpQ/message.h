#ifndef MESSAGE_H
#define MESSAGE_H
#include "message_base.h"

class Message
{
public:
    // default constructor: unbound handle
    Message();
    // attach a handle to a copy of the MsgBase object
    Message(const MessageBase&);
    // copy control members to manage the use count and pointers
    Message(const Message &m);

    ~Message() { decr_use(); }

    Message& operator=(const Message&);

    // member access operators
    const MessageBase *operator->() const;

    MessageBase *operator->();

    const MessageBase &operator*() const;

private:
    // called by both destructor and assignment operator for free pointers
    void decr_use();

    MessageBase *p;     //ָ������Ŀ
    qint32      *use;   // ָ����ʹ����
};

#endif // MESSAGE_H
