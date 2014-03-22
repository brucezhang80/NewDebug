#include "ParseHtml.h"

#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QTimer>
#include <QStringList>
#include <QRegExp>
#include <QEventLoop>

#include <QDebug>

ParseHtml::ParseHtml(QObject *parent) :
    QObject(parent),
    m_pParseManager(new QNetworkAccessManager),
    m_htmlCount(0)
{

}

void ParseHtml::append(const QUrl &url)
{
    if(m_urlVector.contains(url))
        return;
    m_urlVector.push_front(url);

//    if (m_htmlQueue.isEmpty())
//        QTimer::singleShot(1000, this, SLOT(startNextParse()));

    m_htmlQueue.enqueue(url);
    ++m_htmlCount;
    emit htmlCount(m_htmlCount);
}

void ParseHtml::append(const QStringList &urlList)
{
    foreach (QString html, urlList)
        append(QUrl::fromEncoded(html.toLocal8Bit()));

//    if (m_htmlQueue.isEmpty())
//        QTimer::singleShot(2000, this, SIGNAL(finished()));
}

void ParseHtml::startNextParse()
{
    if (m_htmlQueue.isEmpty())
    {
        qDebug() << "All html page done.";
        emit finished();
        return;
    }

<<<<<<< .mine
    QUrl html = m_htmlQueue.dequeue();
    qDebug() << " >>>>>>>>>>>>>>>>>>>> " + html.path();
    QNetworkRequest request(html);
=======
    m_htmlData.clear();
    QUrl url = m_htmlQueue.dequeue();
    qDebug() << m_htmlQueue.count() << url.path();
    QNetworkRequest request(url);
//    QEventLoop loop;
>>>>>>> .r170
    m_pCurParse = m_pParseManager->get(request);

    connect(m_pCurParse, SIGNAL(finished()), this, SLOT(parseFinished()));
    connect(m_pCurParse, SIGNAL(readyRead()), this, SLOT(parseReadyRead()));
//    loop.exec();
}

void ParseHtml::parseReadyRead()
{
    if(m_pCurParse->error() == QNetworkReply::NoError)
    {
        m_htmlData += m_pCurParse->readAll();

    }
}

void ParseHtml::parseFinished()
{
    parseHtmlUrl(m_htmlData);
//    parseImgUrl(m_htmlData);

    if (m_pCurParse->error())
    {
        QString out = QString("Parse html page failed: %1").arg(m_pCurParse->errorString());
        qDebug() << out;
    }
    else
    {
        QString out = QString("Parse html page ok.\n");
        qDebug() << out;
        ++m_htmlCount;
    }

    m_pCurParse->deleteLater();
    startNextParse();
}

void ParseHtml::parseHtmlUrl(const QByteArray &html)
{
    //<img\b[^<>]*?\bsrc[\s\t\r\n]*=[\s\t\r\n]*[""']?[\s\t\r\n]*(?<imgUrl>[^\s\t\r\n""'<>]*)[^<>]*?/?[\s\t\r\n]*>
    QString str = QString(html);
    QString pattern = "/show/([^(|)\"' ]*.html)";
    QRegExp rx(pattern, Qt::CaseInsensitive);
    QStringList list;
    int pos = 0;
    while ((pos = rx.indexIn(str, pos)) != -1)
    {
        QString strUrl = "http://www.6188.com/" + rx.cap(0);
        if( !list.contains(strUrl))
        {
            list << strUrl;
            emit outputLog(strUrl);
        }
        pos += rx.matchedLength();
    }
    append(list);
}

void ParseHtml::parseImgUrl(const QByteArray &html)
{
    //"/([^(|)\"' ]*.jpg)";
    //http://www.6188.com/<a[^\>]*href[^\>]*=\"/([^(|)\\"' ]*.jpg)
    //deskcity:"<img[^\\>]*id=\"s_img\"[\\s\\t\\r\\n]*src=\"/([^(|)\\\"' ]*.jpg)";
    QString str = QString(html);
<<<<<<< .mine
//    qDebug() << str;
    QString pattern("http://pic.2xiezhen.com/([^()\"' ]*)");
    QRegExp rx(pattern, Qt::CaseInsensitive);
//    rx.setMinimal(true);
=======
    QString pattern = "<a[^\\>]*href[^\\>]*=\\\"/([^(|)\\\\\"' ]*.jpg)(<title>(.*)</title>)";
    QString pattern2 = "<title>(.*)</title>";
    QRegExp rx(pattern, Qt::CaseInsensitive);
>>>>>>> .r170
    QStringList list;
    int pos = 0;
    while ( (pos = rx.indexIn(str, pos)) != -1 )
    {
<<<<<<< .mine
        int i = rx.cap(0).lastIndexOf("/");
        if( i != -1)
        {
            QString imgUrl = rx.cap(0).replace(i+1, 1, "");
            list << imgUrl;
            qDebug() << imgUrl;
        }
=======
>>>>>>> .r170
        pos += rx.matchedLength();

        QString imgUrl = "http://www.6188.com/"+rx.cap(1)/*.split(".jpg").at(0) + ".jpg"*/;
        list << imgUrl;
        //emit downloadImg(imgUrl);
    }
    emit updateUrl(list);
}

