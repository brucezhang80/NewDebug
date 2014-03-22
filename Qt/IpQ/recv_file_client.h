#ifndef FILE_CLIENT_H
#define FILE_CLIENT_H

#include <QObject>

class DataClientPrivate;

class RecvFileClient : public QObject
{
    Q_OBJECT
public:

    enum TransferState
    {
        Idle,
        Paused,
        Stoped,
        Reciving,
        Retring
    };
    enum TransferError
    {
        UnknownError,
        FileError,
        ServerError
    };

    RecvFileClient(QObject *parent = 0);
    ~RecvFileClient();

    bool setFileClient(const QString &fileName);
    bool setFileClient(const QByteArray &fileData);

    qint64 progress() const;
    void setReciveBytes(qint64 bytes);
    qint64 reciveBytes() const;
    void setSendBytes(qint64 bytes);
    qint64 sendBytes() const;

    // State and error.
    TransferState state() const;
    QString stateString() const;
    TransferError error() const;
    QString errorString() const;
};

#endif // FILE_CLIENT_H
