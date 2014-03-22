#ifndef OWNER_H
#define OWNER_H

#include <QHostAddress>

class Owner
{
public:
    Owner();
    Owner(QString packet, QHostAddress address, quint16 port);

#if 0
    Owner(const Owner &rhs);
    Owner& operator=(const Owner &rhs);
#endif
    void setName(QString name) { m_name = name; }
    QString name() const { return m_name; }

    void setGroup(QString group) { m_group = group; }
    QString group() const { return m_group; }

    void setLoginName(QString loginName) { m_loginName = loginName; }
    QString loginName() const { return m_loginName; }

    void setHost(QString host) { m_host = host; }
    QString host() const { return m_host; }

    QString ip() const { return m_ipAddress.toString(); }

    void setDisplayLevel(QString displayLevel)
    {
        m_displayLevel = displayLevel;
    }
    QString displayLevel() const { return m_displayLevel; }


private:
    // Get group name or absence(missing, offline) string
    QString parseAdditionalInfo(QString &packet);
    void initOwner(QString &packet);

    QString     m_name;
    QString     m_loginName;
    QString     m_group;
    QString     m_host;
    QHostAddress m_ipAddress;
    quint16     m_port;
    QString     m_displayLevel;
};

#endif // OWNER_H
