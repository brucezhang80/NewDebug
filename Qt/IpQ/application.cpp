#include "application.h"
#include "macai.h"
#include "message_box.h"
#include "systemtray.h"

#include <qt_windows.h>
#include <QLocalSocket>
#include <QTextStream>
#include <QStringList>
#include <QFile>
#include <QString>

Application::Application(int argc, char **argv)
    :QApplication(argc, argv)
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

bool Application::isRunning()
{
    return m_isRunning;
}

bool Application::winEventFilter(MSG *msg, long *result)
{
    if(msg->message == WM_APP + 101)
    {
        //�����ͼ���ϵ�hover�¼�
        QPoint mouse_pt(msg->pt.x, msg->pt.y);
        QPoint mul_pt = Macai::systemTray->geometry().center() - mouse_pt;
        if( mul_pt.x() > mouse_pt.y() )
        {
            // ����ƽ�����ʱ,��ʱ����xֵС���Ƴ�ʱ��xֵ,yֵһֱ�Ǳ��ֲ���
            // ����������x���������x���,�������y����,��ǰ������״̬.
            //Macai::systemTray->showTooltip(false);
            Macai::messageBox->show();
        }
        else
        {
            //Macai::systemTray->showTooltip(true);
            Macai::messageBox->hide();
        }
    }

    return QApplication::winEventFilter(msg, result);
}

void Application::newLocalConnection()
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
