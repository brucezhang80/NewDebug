/**************************************************************************
** Qt Creator license header template
**   Special keywords: Administrator 2013/12/11 2013
**   Environment variables:
**   To protect a percent sign, use '%'.
**************************************************************************/

#include "XiaoMi.h"

XiaoMi::XiaoMi(QObject *parent) :
    NetWorkLogin(parent)
{
    m_bLogin = false;
    m_loginUrl = QString("https://account.xiaomi.com/pass/serviceLoginAuth2");
    m_username = QString("");
    m_password = QString("");
    m_headerName = "User-Agent";
    m_headerValue = "Mozilla/4.0 (compatible; MSIE 5.5; Windows NT)";
    m_hdInfoUrl = "http://tc.hd.xiaomi.com/hdinfo?callback=hdinfo&_";

    m_netManager = new QNetworkAccessManager(this);
    m_cookieJar = new NetworkCookieJar(this);
//    m_request = new QNetworkRequest;
    m_netManager->setCookieJar(m_cookieJar);

    connect(m_netManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(finished(QNetworkReply*)));
}

XiaoMi::XiaoMi(const QString &loginUrl, const QString &username,
               const QString &password, QObject *parent) :
    NetWorkLogin(parent),
    m_loginUrl(loginUrl),
    m_username(username),
    m_password(password)
{
    m_bLogin = false;
    m_headerName = "User-Agent";
    m_headerValue = "Mozilla/4.0 (compatible; MSIE 5.5; Windows NT)";
    m_hdInfoUrl = "http://tc.hd.xiaomi.com/hdinfo?callback=hdinfo&_";
    m_requestData = QString("passToken=&user=" + m_username +
                            "&_json=true&pwd=" + m_password +
                            "&sid=passport&_sign=KKkRvCpZoDC%2BgLdeyOsdMhwV0Xg%3D"
                            "&callback=https%3A%2F%2Faccount.xiaomi.com"
                            "&qs=%253Fsid%253Dpassport&hidden=")
                            .toLocal8Bit();

    m_netManager = new QNetworkAccessManager(this);
    m_cookieJar = new NetworkCookieJar(this);
    m_netManager->setCookieJar(m_cookieJar);

    connect(m_netManager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(replyFinished(QNetworkReply*)));
}

void XiaoMi::setRawHeader(const QByteArray &headerName,
                          const QByteArray &headerValue)
{
    m_headerName = headerName;
    m_headerValue = headerValue;
}

QString XiaoMi::hdInfoText()
{
    if( m_bLogin )
    {
        QNetworkRequest request;
        QNetworkAccessManager *nam = new QNetworkAccessManager();
        request.setUrl(QUrl(m_hdInfoUrl));
        QNetworkReply* reply = nam->get(request);

        QEventLoop eventLoop;
        connect(nam, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()) );
        eventLoop.exec();       //block until finish

        QByteArray responseData = reply->readAll();

        return QString(responseData);
    }
    return QString("");
}

bool XiaoMi::hasLogin()
{

    return false;
}

bool XiaoMi::hasStarted()
{

    return false;
}

bool XiaoMi::hasBuyUrl()
{

    return false;
}

void XiaoMi::login()
{
    if( m_bLogin ) return;
    login(m_loginUrl, m_username, m_password, m_requestData);
}

void XiaoMi::login(const QString &loginUrl,
                   const QString &username,
                   const QString &password)
{
    if( m_bLogin ) return;

    m_loginUrl = loginUrl;
    m_username = username;
    m_password = password;

    QNetworkRequest request;
    request.setUrl(QUrl(loginUrl));
    request.setRawHeader(m_headerName, m_headerValue);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      "application/x-www-form-urlencoded");
    m_netManager->post(request, m_requestData);
}

void XiaoMi::login(const QString &loginUrl, const QString &username,
                   const QString &password, const QByteArray &requestData)
{
    if( m_bLogin ) return;

    m_loginUrl = loginUrl;
    m_username = username;
    m_password = password;
    m_requestData = requestData;

    QNetworkRequest request;
    request.setUrl(QUrl(loginUrl));
    request.setRawHeader(m_headerName, m_headerValue);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      "application/x-www-form-urlencoded");
    m_netManager->post(request, requestData);
}

void XiaoMi::finished(QNetworkReply *reply)
{
    if(reply && reply->error() == QNetworkReply::NoError)
    {
        if( !m_bLogin )
        {
            QByteArray data = reply->readAll();
            qDebug() << data;
            m_cookiesList = m_cookieJar->cookies();
            for(int i = 0; i < m_cookiesList.count(); ++i)
            {
                qDebug() << m_cookiesList.at(i).toRawForm();
            }
        }
    }

    emit this->error((int)reply->error());
    qDebug() << reply->errorString();

    reply->close();
}

