#include "message.h"

Message::Message()
    :p(0), use(new qint32(1))
{
}

Message::Message(const MessageBase &m)
    : p(m.clone()), use(new qint32(1))
{
}

Message::Message(const Message &m)
    : p(m.p), use(m.use)
{
    ++*use;
}

Message &Message::operator =(const Message &rhs)
{
    ++*rhs.use;
    decr_use();

    p = rhs.p;
    use = rhs.use;

    return *this;
}


const MessageBase *Message::operator ->() const
{
    if (p)
    {
        return p;
    }
    else
    {
        throw std::logic_error("unbound Msg");
    }
}

MessageBase *Message::operator ->()
{
    if (p)
    {
        return p;
    }
    else
    {
        throw std::logic_error("unbound Msg");
    }
}

const MessageBase &Message::operator *() const
{
    if (p)
    {
        return *p;
    }
    else
    {
        throw std::logic_error("unbound Msg");
    }
}

void Message::decr_use()
{
    if (--*use == 0)
    {
        delete p;
        delete use;
    }
}
