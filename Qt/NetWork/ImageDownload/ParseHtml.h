#ifndef PARSEHTML_H
#define PARSEHTML_H

#include <QObject>
#include <QUrl>
#include <QQueue>
#include <QVector>

class QNetworkAccessManager;
class QNetworkReply;

class ParseHtml : public QObject
{
    Q_OBJECT
public:
    explicit ParseHtml(QObject *parent = 0);
    void    append(const QUrl &url);
    void    append(const QStringList &urlList);

    void parseHtmlUrl(const QByteArray &html);
    void parseImgUrl(const QByteArray &html);

signals:
    void finished();
    void updateUrl(const QStringList &urls);
    void outputLog(const QString &url);
    void htmlCount(int count);

public slots:
    void startNextParse();
    void parseReadyRead();
    void parseFinished();

private:
    QNetworkAccessManager*      m_pParseManager;
    QNetworkReply*              m_pCurParse;
    QQueue<QUrl>                m_htmlQueue;
    QVector<QUrl>               m_urlVector;

    int                         m_htmlCount;
    QByteArray                  m_htmlData;
};

#endif // PARSEHTML_H
