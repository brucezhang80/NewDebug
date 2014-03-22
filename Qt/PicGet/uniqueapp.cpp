#include "uniqueapp.h"
#include <QLocalSocket>
#include <QTextStream>
#include <QStringList>
#include <QFile>

UniqueApp::UniqueApp(int argc, char **argv):
    QApplication(argc, argv)
{
    m_isRunning = false;
    QCoreApplication::setApplicationName("localserver");
    QString serverName = QCoreApplication::applicationName();

    QLocalSocket socket;
    socket.connectToServer(serverName);

    if( socket.waitForConnected(500) )
    {
        QTextStream stream(&socket);
        QStringList args=QCoreApplication::arguments();

        if( args.count() > 1 )
        {
            stream<<args.last();
        }
        else
        {
            stream<<QString();
        }
        stream.flush();
        qDebug()<<"Connected server,program will quit";
        socket.waitForBytesWritten();

        /**
        *qApp->quit(); �˴����������˳��¼�ѭ����;�ڹ��캯����,�¼�ѭ��
        *��δ����,��˾��޷��˳�. ��þ������ñ�־�ж����ⲿ�ж�
        */
        m_isRunning=true;
        return;
    }
    qDebug()<<"Can't connect to server,build a server";
    m_server = new QLocalServer(this);
    connect(m_server, SIGNAL(newConnection()), this, SLOT(newLocalConnection()));

    if( m_server->listen(serverName) )
    {
        //��ֹ�������ʱ,�������̷���,�Ƴ�֮
        if(m_server->serverError() == QAbstractSocket::AddressInUseError&&QFile::exists( m_server->serverName()) )
        {
            QFile::remove(m_server->serverName());
            m_server->listen(serverName);
        }
    }
}

bool UniqueApp::isRunning()
{
    return m_isRunning;
}

void UniqueApp::newLocalConnection()
{
    QLocalSocket *socket = m_server->nextPendingConnection();
    if(!socket)
        return;
    socket->waitForReadyRead(1000);

    //��ʾ�������ֵ
    QTextStream in(socket);
    QString vl;
    in >> vl;
    qDebug()<<"The value is: "<<vl;

    delete socket;
}
