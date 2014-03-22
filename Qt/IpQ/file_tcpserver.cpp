#include "file_tcpserver.h"
#include "ipq.h"
#include "send_file_thread.h"

FileTcpServer::FileTcpServer(QObject *parent) :
    QTcpServer(parent)
{
    startListening();
}

void FileTcpServer::incomingConnection(int socketDescriptor)
{
    SendFileThread *sendFileThread = new SendFileThread(socketDescriptor, this);

    connect(sendFileThread, SIGNAL(finished()),
            sendFileThread, SLOT(deleteLater()));

    sendFileThread->start();
}

void FileTcpServer::startListening()
{
    listen(QHostAddress::Any, IPQ_DEFAULTPORT);
}
