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
        *qApp->quit(); 此代码是用来退出事件循环的;在构造函数中,事件循环
        *尚未启动,因此就无法退出. 最好就是设置标志判断在外部判断
        */
        m_isRunning=true;
        return;
    }
    qDebug()<<"Can't connect to server,build a server";
    m_server = new QLocalServer(this);
    connect(m_server, SIGNAL(newConnection()), this, SLOT(newLocalConnection()));

    if( m_server->listen(serverName) )
    {
        //防止程序崩溃时,残留进程服务,移除之
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
        //鼠标在图标上的hover事件
        QPoint mouse_pt(msg->pt.x, msg->pt.y);
        QPoint mul_pt = Macai::systemTray->geometry().center() - mouse_pt;
        if( mul_pt.x() > mouse_pt.y() )
        {
            // 鼠标移进托盘时,此时鼠标的x值小于移出时的x值,y值一直是保持不变
            // 当托盘坐标x与鼠标坐标x相减,大于鼠标y坐标,则当前是移入状态.
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

    //显示传入参数值
    QTextStream in(socket);
    QString vl;
    in >> vl;
    qDebug()<<"The value is: "<<vl;

    delete socket;
}
