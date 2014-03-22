#ifndef RECV_FILE_TRANSFER_H
#define RECV_FILE_TRANSFER_H

#include <QObject>
#include <QMutex>
#include <QWaitCondition>
#include <QObject>
#include <QTcpSocket>

class RecvFileClient;
class QFile;

struct TransferFile
{
    QString name;
    qint64 size;
    int type;
    QMap<int, QString> extendAttr;
};

class RecvFileTransfer : public QObject
{
    Q_OBJECT
public:
    explicit RecvFileTransfer(RecvFileClient *recvFileClien, QObject *parent = 0);
    void resumeTransfer();

    void startTransfer();

signals:
    void recvFileFinished();
    void recvFileError(QString);

public slots:
    void stopTransfer();
    void abortTransfer();

private:
    QByteArray constructRecvFileDatagram();
    bool recvFileRegular();
    bool recvFileDir();
    bool saveData(QByteArray recvBlock, QFile &file);
    bool canParseHeader(QByteArray &recvBlock);
    bool parseHeader(QByteArray &recvBlock,
                     struct TransferFile &transferFile);

    QMutex          m_lock;
    QWaitCondition  m_cond;
    RecvFileClient  *m_recvFileClient;
    QString         m_errorString;

    QTcpSocket      m_tcpSocket;

    bool            isStopTransfer;
    bool            isAbortTransfer;
};

#endif // RECV_FILE_TRANSFER_H
