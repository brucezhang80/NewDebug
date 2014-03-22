#include "recv_file_client.h"

RecvFileClient::RecvFileClient(QObject *parent)
{
}

RecvFileClient::~RecvFileClient()
{
}

bool RecvFileClient::setFileClient(const QString &fileName)
{
    return true;

}

bool RecvFileClient::setFileClient(const QByteArray &fileData)
{

    return true;
}

qint64 RecvFileClient::progress() const
{
    return 0;
}

void RecvFileClient::setReciveBytes(qint64 bytes)
{
}

qint64 RecvFileClient::reciveBytes() const
{
    return 0;
}

void RecvFileClient::setSendBytes(qint64 bytes)
{
}

qint64 RecvFileClient::sendBytes() const
{
    return 0;
}

RecvFileClient::TransferState RecvFileClient::state() const
{
    return Idle;
}

QString RecvFileClient::stateString() const
{
    return "Idle";
}

RecvFileClient::TransferError RecvFileClient::error() const
{
    return UnknownError;
}

QString RecvFileClient::errorString() const
{
    return "UnknownError";
}
