#ifndef HTTPDOWNLOAD_H
#define HTTPDOWNLOAD_H

#include <QObject>
#include <QNetworkRequest>
#include <QMap>
#include <QUrl>
#include <QFile>
#include <QFileInfo>
#include <QQueue>
#include <QTime>

class ParseHtml;
class QPixmap;
class QByteArray;
class QThread;
class QEventLoop;
class QNetworkAccessManager;
class QNetworkReply;
class QTcpSocket;

struct TaskInfomation
{
    int     taskId;
    QUrl    url;
    QString filename;
    qint64  size;
    qint64  completed;
    qint64  speed;
};

class HttpDownload : public QObject
{
    Q_OBJECT
public:
    explicit HttpDownload(QObject *parent = 0);
    ~HttpDownload();

    void    download(QUrl(url));
    qint64  fileSize(QUrl(url));
    QString localFilename(const QUrl &url);

signals:
    void updateDownloadProgressbar(qint64, qint64);
    void finished();
    void outputLog(const QString &log);
    void htmlCount(int count);
    void imgCount(int count);

public slots:
    void append(const QUrl &url);
    void append(const QStringList &urlList);

private slots:
    void startNextDownload();
    void downloadFinished();
    void downloadReadyRead();
    void updateReadProgress(qint64, qint64);

private:
    QNetworkAccessManager*      m_downloadManager;
    QNetworkReply*              m_curDownloading;
    QList<TaskInfomation>       m_tasks;
    ParseHtml*                  m_parseHtml;

    QFile                       m_file;
    QTime                       downloadTime;
    QQueue<QUrl>                m_downloadQueue;
    int                         m_completedImgCount;
    int                         m_completedHtmlCount;
    int                         m_totalCount;
};

#endif // HTTPDOWNLOAD_H
