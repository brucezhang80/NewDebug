#include "owner.h"
#include "constants.h"
#include "ipq.h"

#include <QStringList>

Owner::Owner()
{
}

Owner::Owner(QString packet, QHostAddress address, quint16 port)
    : m_ipAddress(address), m_port(port)
{
    initOwner(packet);
}

#if 0
Owner::Owner(const Owner &rhs)
{
    m_name = rhs.m_name;
    m_loginName = rhs.m_loginName;
    m_group = rhs.m_group;
    m_host = rhs.m_host;
    m_ipAddress = rhs.m_ipAddress;
    m_port = rhs.m_port;
    m_displayLevel = rhs.m_displayLevel;
}

Owner& Owner::operator=(const Owner &rhs)
{
    if (&rhs == this) {
        return *this;
    }

    m_name = rhs.m_name;
    m_loginName = rhs.m_loginName;
    m_group = rhs.m_group;
    m_host = rhs.m_host;
    m_ipAddress = rhs.m_ipAddress;
    m_port = rhs.m_port;
    m_displayLevel = rhs.m_displayLevel;

    return *this;
}
#endif

void Owner::initOwner(QString &packet)
{
    QStringList list = packet.split( CommandSeperator );
    if( list.size() < MsgNormalFieldCount )
        return;

    m_loginName = list.at( MsgLogNamePos );
    quint32 flag = list.at( MsgFlagsPos ).toUInt();
    if( (flag & IPQ_BROADCAST_ENTRY) || (flag & IPQ_BROADCAST_ABSENCE) )
    {
        m_group = packet.section( QChar(ExtendInfoSeperator), 1, 1);
        m_name = parseAdditionalInfo(packet);
        if( m_name.isEmpty() )
            m_name = m_loginName;
    }

    if( (flag & IPQ_SEND_MESSAGE) )
        m_name = m_loginName;

    m_host = list.at( MsgHostPos );
}

QString Owner::parseAdditionalInfo(QString &packet)
{
    int cnt = 0;
    int index = 0;
    while ( cnt < MsgAdditionInfoPos )
    {
        int id = packet.indexOf(QChar( CommandSeperator ), index);
        if (index == -1)
        {
            break;
        }
        ++cnt;
        index = id + 1;
    }

    QString s = packet.right(packet.size() - index);

    return s.section(QChar( ExtendInfoSeperator ), 0, 0);
}
