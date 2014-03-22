#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QThread>
#include <QBitArray>
#include <QFileInfoList>


class FileManager : public QThread
{
    Q_OBJECT
public:
    explicit FileManager(QObject *parent = 0);
    virtual ~FileManager();

    inline void setDestinationFolder(const QString &directory) { destinationPath = directory; }
    inline void setFileName(const QString &filename){ srcfileName = filename; }
    //void write(int pieceIndex, int offset, const QByteArray &data);
    void copy();
    void getFolderFiles(QString dir, QFileInfoList &fList);

    bool isFile(const QString &src);
    bool isPath(const QString &src);

    void copyFolder(QString scrPath, QString dstPath);
    void copyFile(QString srcFile, QString dstPath);

protected:
    void run();

signals:
    void verificationProgressSignal(int percent);
    void bytesReceivedSignal(qint64 bytes);

public slots:

private:

    bool newFile;
    QString destinationPath;
    QString srcfileName;

    QBitArray verifiedPieces;
    QList<int> pendingVerificationRequests;
    QList<int> newPendingVerificationRequests;
    QList<qint64> fileSizes;
    qint64  fileSize;
};

#endif // FILEMANAGER_H
