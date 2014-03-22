/**************************************************************************
** Qt Creator license header template
**   Special keywords: Administrator 2013/12/11 2013
**   Environment variables:
**   To protect a percent sign, use '%'.
**************************************************************************/

#ifndef TAOBAO_H
#define TAOBAO_H

#include <QObject>
#include <QtNetwork>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QByteArray>

#include "NetWorkLogin.h"

class TaoBao : public NetWorkLogin
{
    Q_OBJECT
public:
    explicit TaoBao(QObject *parent = 0);
    TaoBao(const QString &loginUrl,
           const QString &username,
           const QString &password,
           QObject *parent = 0);

    ~TaoBao(){}

    void setLoginUrl(const QString &url){ m_loginUrl = url;}
    QString loginUrl() const { return m_loginUrl;}

    void setUsername(const QString &uername){ m_username = uername;}
    QString username() const { return m_username;}

    void setPassword(const QString &password){ m_password = password;}
    QString password() const { return m_password;}

    void setRequestData(const QByteArray &requestData){ m_requestData = requestData;}
    QString requestData() const {return m_requestData;}

    void setRawHeader(const QByteArray &headerName, const QByteArray &headerValue);
    QByteArray rawHeader() const {return (m_headerName+m_headerValue);}

    void setNetworkAccessManager(QNetworkAccessManager* nam){ m_netManager= nam;}
    QNetworkAccessManager*  networkAccessManager() const{ return m_netManager;}
    void setNetworkCookieJar(NetworkCookieJar* cookieJar){m_cookieJar = cookieJar;}
    NetworkCookieJar* networkCookieJar() const { return m_cookieJar;}

signals:
    void error(int errcode);
    void loginReady(const QNetworkRequest &request, const QByteArray &requestData);

public slots:
    void login();
    void login(const QString &loginUrl,
                       const QString &username,
                       const QString &password);

    void login(const QString &loginUrl,
                       const QString &username,
                       const QString &password,
                       const QByteArray &requestData);

private slots:
    void finished(QNetworkReply* reply);

private:
    QString                 m_loginUrl;
    QString                 m_username;
    QString                 m_password;
    QByteArray              m_requestData;

    QByteArray              m_headerName;
    QByteArray              m_headerValue;

    NetworkCookieJar*       m_cookieJar;
    QList<QNetworkCookie>   m_cookiesList;
    QNetworkAccessManager*  m_netManager;

};

#endif // TAOBAO_H