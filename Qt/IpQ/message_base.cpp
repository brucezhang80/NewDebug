#include "macai.h"
#include "ipq.h"
#include "constants.h"
#include "system.h"
#include "user_manager.h"
#include "message_base.h"
#include <QStringList>

MessageBase::MessageBase()
{
}

MessageBase::~MessageBase()
{
}

MessageBase::MessageBase(QString packet, QHostAddress address, quint16 port)
    : m_owner(packet, address, port), m_packet(packet), m_ipAddress(address), m_port(port)
{
    parsePacket();
}

MessageBase::MessageBase(QHostAddress address, quint16 port, QString additionalInfo,
                         QString extendedInfo, quint32 flags)
    :m_ipAddress(address), m_port(port), m_additionalInfo(additionalInfo),
      m_extendedInfo(extendedInfo), m_flags(flags)
{
    m_packetNoString = System::packetNoString();
    m_owner = Macai::userManager->ownerSelf();

    constructPacket();
}

void MessageBase::parsePacket()
{
    QStringList list = m_packet.split( CommandSeperator );

    // 检查当前数据包是否满足数据量
    if ( list.count() < MsgNormalFieldCount )
        return;

    m_extendedInfo = m_packet.section(QChar( ExtendInfoSeperator ), 1, 1);
    parseAdditionalInfo();

    m_packetNoString = list.at( MsgPacketNoPos );
    m_flags = list.at( MsgFlagsPos ).toInt(); // 4
}

// 解析额外的数据信息
void MessageBase::parseAdditionalInfo()
{
    int cnt = 0;
    int index = 0;
    while (cnt < MsgAdditionInfoPos) // 5
    {
        int id = m_packet.indexOf(QChar( CommandSeperator ), index);
        if (index == -1)
        {
            break;
        }
        ++cnt;
        index = id + 1;
    }

    if (index == -1)
    {
        m_additionalInfo = "";
    }
    else
    {
        QString s = m_packet.right(m_packet.size() - index);
        m_additionalInfo = s.section(QChar( ExtendInfoSeperator ), 0, 0);
    }
}

// 包中含有 版本号 登录名 主机地址 标志号 一些其他信息
// "version":"":"logname":"host":"flag":"addition"
void MessageBase::constructPacket()
{
    //CommandSeperator = ":"
    // 0
    m_packet.append(QString("%1%2").arg( IPQ_VERSION ).arg( CommandSeperator ));

    // 1
    m_packet.append(QString("%1%2").arg(m_packetNoString)
                    .arg( CommandSeperator ));

    // 2
    m_packet.append(m_owner.loginName());
    m_packet.append( CommandSeperator );
    // 3
    m_packet.append(m_owner.host());
    m_packet.append( CommandSeperator );
    // 4
    m_packet.append(QString("%1%2").arg(m_flags).arg( CommandSeperator ));
    // 5
    m_packet.append(m_additionalInfo);
}

