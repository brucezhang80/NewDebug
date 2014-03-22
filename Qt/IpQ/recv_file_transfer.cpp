#include "recv_file_transfer.h"
#include "recv_file_client.h"
#include "macai.h"
#include "constants.h"
#include "system.h"
#include "user_manager.h"

#include <QFile>
#include <QDir>
#include <QTextCodec>

#include <sys/types.h>
#include <sys/utime.h>


RecvFileTransfer::RecvFileTransfer(RecvFileClient *recvFileClien, QObject *parent)
    : QObject(parent), m_recvFileClient(recvFileClien),
    isStopTransfer(false), isAbortTransfer(false)
{
}

void RecvFileTransfer::startTransfer()
{
    qDebug() << "RecvFileTransfer::startTransfer()";
}

bool RecvFileTransfer::recvFileRegular()
{
    return false;
}

bool RecvFileTransfer::recvFileDir()
{

    return false;
}

bool RecvFileTransfer::parseHeader(QByteArray &recvBlock, struct TransferFile &transferFile)
{
    bool ok;
    int headerSize
        = recvBlock.left(TRANSFER_FILE_HEADER_SIZE_LENGTH).toInt(&ok, 16);
    if (!ok)
    {
        m_errorString = "RecvFileTransfer::parseHeader: get headerSize error";
        return false;
    }

    QByteArray header = recvBlock.left(headerSize);

    QList<QByteArray> list = header.split(':');

#define TRANSFERFILE_NAME_POS           1
#define TRANSFERFILE_SIZE_POS           2
#define TRANSFERFILE_TYPE_POS           3
#define TRANSFERFILE_ATTR_BEGIN_POS     4

    // XXX NOTE: canParseHeader() make sure we have enough items in list,
    // so we do not need to check size of list before call list.at()
//    transferFile.name = Macai::transferCodec->codec()->toUnicode(list.at(TRANSFERFILE_NAME_POS));
    QTextCodec* codec = QTextCodec::codecForName("SYSTEM");
    transferFile.name = codec->toUnicode(list.at(TRANSFERFILE_NAME_POS));

    transferFile.size = list.at(TRANSFERFILE_SIZE_POS).toLongLong(&ok, 16);
    if (!ok)
    {
        m_errorString = "RecvFileTransfer::parseHeader: get file size error";
        return false;
    }
    transferFile.type = list.at(TRANSFERFILE_TYPE_POS).toInt(&ok, 16);
    if (!ok)
    {
        m_errorString = "RecvFileTransfer::parseHeader: get file type error";
        return false;
    }

    // Extended file attribution like mtime, atime...
    for (int i = TRANSFERFILE_ATTR_BEGIN_POS; i < list.size(); ++i)
    {
        QString s = list.at(i);
        QStringList l = s.split(QChar('='));
        if (l.size() == 2)
        {
            int i = l.at(0).toInt(&ok, 16);
            if (ok)
            {
                transferFile.extendAttr.insert(i, l.at(1));
            }
        }
    }

    recvBlock.remove(0, headerSize);

    return true;
}

bool RecvFileTransfer::canParseHeader(QByteArray &recvBlock)
{
    if (recvBlock.size() < TRANSFER_FILE_HEADER_SIZE_LENGTH)
        return false;

    bool ok;
    int headerSize = recvBlock.left(TRANSFER_FILE_HEADER_SIZE_LENGTH).toInt(&ok, 16);

    if (recvBlock.size() >= headerSize)
    {
        return true;
    }

    return false;
}

bool RecvFileTransfer::saveData(QByteArray recvBlock, QFile &file)
{
    qint64 bytesToWrite = recvBlock.size();

    forever
    {
        qint64 bytesWrited = file.write(recvBlock);

        if (bytesWrited == -1)
        {
            m_errorString = file.errorString();
            return false;
        }
        bytesToWrite -= bytesWrited;
        if (bytesToWrite == 0)
        {
            break;
        }
        recvBlock.remove(0, bytesWrited);
    }

    return true;
}

QByteArray RecvFileTransfer::constructRecvFileDatagram()
{
    QString s = QString("%1:%2:%3:%4").arg(IPQ_VERSION)
        .arg(System::packetNoString())
        .arg(Macai::userManager->ownerSelf().loginName())
        .arg(Macai::userManager->ownerSelf().host());

    QByteArray ba;
    ba.append(s);
    return ba;
}

void RecvFileTransfer::stopTransfer()
{
    qDebug() << "RecvFileTransfer::stopTransfer";

    isStopTransfer = true;
}

void RecvFileTransfer::abortTransfer()
{
    qDebug() << "RecvFileTransfer::abortTransfer";

    isAbortTransfer = true;
}

void RecvFileTransfer::resumeTransfer()
{
    qDebug() << "RecvFileTransfer::resumeTransfer";

    if (isStopTransfer)
    {
        // XXX NOTE: we need first set 'isStopTransfer' to false to avoid
        // m_cond.wait() be called.
        m_lock.lock();
        isStopTransfer = false;
        m_cond.wakeOne();
        m_lock.unlock();
    }
}
