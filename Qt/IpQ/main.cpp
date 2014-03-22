#include "main_window.h"
#include "systemtray.h"
#include "ipq.h"
#include "macai.h"
#include "system.h"
#include "message_thread.h"
#include "constants.h"
#include "user_manager.h"
#include "file_tcpserver.h"

#include "application.h"
#include <QApplication>
#include <QTextCodec>

static void checkTcpServerError();

int main(int argc, char *argv[])
{
    // ����ʱʹ�� Application app(argc, argv); �Ա�ֻ������һ��ʵ��
    Application app(argc, argv);
    QApplication::addLibraryPath("./plugins");
//    QApplication app(argc, argv);
    QTextCodec* codec = QTextCodec::codecForName("SYSTEM");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
    if( app.isRunning() )
    {
        QMessageBox::information( 0, QObject::tr("Warning"), QObject::tr("Only one application run.") );
        return 0;
    }

    app.setQuitOnLastWindowClosed(false);//���һ�����ڹر�ʱ,���˳�����

    QString homePath =  QApplication::applicationDirPath();
    System::setAppPath(homePath);
    Macai::MacaiStart(homePath);

    checkTcpServerError();

    Macai::messageThread->start();
    while( !Macai::messageThread->isRunning() )
    {
        qDebug() << "not Running";
//        Macai::messageThread->usleep(200000);
    }

    Macai::userManager->broadcastEntry();

    int rc = app.exec();

    Macai::userManager->broadcastExit();

    Macai::MacaiEnd();

    return rc;
}

static void checkTcpServerError()
{
    // Check tcp server
    if (!Macai::fileTcpServer->isListening())
    {
        QString errorString(QObject::tr("Start tcp server error!"));
        QMessageBox::critical(0, QObject::tr("Start QIpMsg"),
                          errorString + ":\n"
                          + Macai::fileTcpServer->errorString());

        qDebug() << "main::checkTcpServerError:"
            << Macai::fileTcpServer->errorString();

        exit(-1);
    }
}
