/**************************************************************************
** Qt Creator license header template
**   Special keywords: Administrator 2013/12/11 2013
**   Environment variables:
**   To protect a percent sign, use '%'.
**************************************************************************/

#include "TaoBao.h"

TaoBao::TaoBao(QObject *parent) :
    NetWorkLogin(parent)
{
    m_loginUrl = QString("https://login.taobao.com");
    m_username = QString("");
    m_password = QString("");
    m_headerName = "User-Agent";
    m_headerValue = "Mozilla/4.0 (compatible; MSIE 5.5; Windows NT)";

    m_netManager = new QNetworkAccessManager(this);
    m_cookieJar = new NetworkCookieJar(this);
    m_netManager->setCookieJar(m_cookieJar);

    connect(m_netManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(finished(QNetworkReply*)));
}

TaoBao::TaoBao(const QString &loginUrl, const QString &username,
               const QString &password, QObject *parent):
    NetWorkLogin(parent),
    m_loginUrl(loginUrl),
    m_username(username),
    m_password(password)
{
    m_headerName = "User-Agent";
    m_headerValue = "Mozilla/4.0 (compatible; MSIE 5.5; Windows NT)";

    m_requestData = QString("TPL_username=%1&TPL_password=%2"
                            "&actionForStable=enable_post_user_action&action=Authenticator&event_submit_do_login=anything")
                            .arg(m_username).arg(m_password).toLocal8Bit();

    m_netManager = new QNetworkAccessManager(this);
    m_cookieJar = new NetworkCookieJar(this);
    m_netManager->setCookieJar(m_cookieJar);

    connect(m_netManager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(replyFinished(QNetworkReply*)));
}

void TaoBao::setRawHeader(const QByteArray &headerName, const QByteArray &headerValue)
{
    m_headerName = headerName;
    m_headerValue = headerValue;
}

void TaoBao::login()
{
    login(m_loginUrl, m_username, m_password, m_requestData);
}

void TaoBao::login(const QString &loginUrl,
                   const QString &username,
                   const QString &password)
{
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

void TaoBao::login(const QString &loginUrl, const QString &username,
                   const QString &password, const QByteArray &requestData)
{
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
    emit loginReady(request, requestData);
}

void TaoBao::finished(QNetworkReply *reply)
{
    if(reply && reply->error() == QNetworkReply::NoError)
    {
        QByteArray data = reply->readAll();
        qDebug() << data;
        m_cookiesList = m_cookieJar->cookies();
        for(int i = 0; i < m_cookiesList.count(); ++i)
        {
            qDebug() << m_cookiesList.at(i).toRawForm() << m_cookiesList.at(i).path();
        }
    }

    emit this->error((int)reply->error());
//    qDebug() << reply->errorString();

    reply->close();
}

