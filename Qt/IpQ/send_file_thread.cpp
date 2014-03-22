#include "send_file_thread.h"


#include <QFile>
#include <QtDebug>
#include <QFileInfo>
#include <QTextCodec>
#include <QTextCodec>
#include <QDir>
#include <QDateTime>

SendFileThread::SendFileThread(QObject *parent)
{
}

SendFileThread::SendFileThread(int socketDescriptor, QObject *parent) :
    QThread(parent), m_socketDescriptor(socketDescriptor)
{
}

void SendFileThread::run()
{
    qDebug("SendFileThread::run: begin");

    qDebug() << "Thread : File transfer completed";
}

