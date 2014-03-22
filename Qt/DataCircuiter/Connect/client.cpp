#include <QtNetwork>
#include <QDebug>
#include <QMessageBox>
#include "connection.h"
//#include "peermanager.h"
#include "client.h"


Client::Client()
{
    QObject::connect(&server, SIGNAL(newConnection(Connection*)), this, SLOT(newConnection(Connection*)));
    //QMessageBox::information(0, "Chat", "3dsfds");
}

void Client::sendMessage(const QString &message)
{
    if (message.isEmpty())
        return;
    QList<Connection *> connections = peers.values();
    foreach (Connection *connection, connections)
        connection->sendMessage(message);
}

QString Client::nickName() const
{
    QByteArray username;
    QStringList envVariables;
    envVariables << "USERNAME.*" << "USER.*" << "USERDOMAIN.*" << "HOSTNAME.*" << "DOMAINNAME.*";

    QStringList environment = QProcess::systemEnvironment();
    foreach (QString string, envVariables)
    {
        int index = environment.indexOf(QRegExp(string));
        if (index != -1)
        {
            QStringList stringList = environment.at(index).split('=');
            if (stringList.size() == 2)
            {
                username = stringList.at(1).toUtf8();
                break;
            }
        }
    }

    return ( username+ '@' + QHostInfo::localHostName()+ ':' + QString::number(server.serverPort()) );
}

bool Client::hasConnection(const QHostAddress &senderIp, int senderPort) const
{
    if (senderPort == -1)
    {
        return peers.contains(senderIp);
    }

    if (!peers.contains(senderIp))
        return false;

    QList<Connection *> connections = peers.values(senderIp);
    foreach (Connection *connection, connections)
    {
        if (connection->peerPort() == senderPort)
        return true;
    }

    return false;
}

void Client::newConnection(Connection *connection)
{
    QMessageBox::information(0, "Chat", "new Connection.");

    connect(connection, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectionError(QAbstractSocket::SocketError)));
    connect(connection, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(connection, SIGNAL(readyForUse()), this, SLOT(readyForUse()));
}

void Client::connectionError(QAbstractSocket::SocketError /*socketError*/)
{
    if (Connection *connection = qobject_cast<Connection *>(sender()))
        removeConnection(connection);
}

void Client::disconnected()
{
    if (Connection *connection = qobject_cast<Connection *>(sender()))
        removeConnection(connection);
}

void Client::readyForUse()
{
    Connection *connection = qobject_cast<Connection *>(sender());
    if (!connection || hasConnection(connection->peerAddress(), connection->peerPort()))
        return;

    connect(connection, SIGNAL(newMessage(QString,QString)), this, SIGNAL(newMessage(QString,QString)));

    peers.insert(connection->peerAddress(), connection);
    QString nick = connection->name();
    if (!nick.isEmpty())
        emit newParticipant(nick);

    QMessageBox::information(0, "Chat", "new participant.");
}

void Client::removeConnection(Connection *connection)
{
    if (peers.contains(connection->peerAddress()))
    {
        peers.remove(connection->peerAddress());
        QString nick = connection->name();
        if (!nick.isEmpty())
        emit participantLeft(nick);
    }
    connection->deleteLater();
}
