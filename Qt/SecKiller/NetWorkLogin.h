/**************************************************************************
** Qt Creator license header template
**   Special keywords: Administrator 2013/12/11 2013
**   Environment variables:
**   To protect a percent sign, use '%'.
**************************************************************************/

#ifndef NETWORKLOGIN_H
#define NETWORKLOGIN_H

#include <QObject>
#include <QtNetwork>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QByteArray>

#include "NetworkCookieJar.h"

class NetWorkLogin : public QObject
{
    Q_OBJECT
public:
    explicit NetWorkLogin(QObject *parent = 0);

    // pure virtual interface
//    virtual ~NetWorkLogin() = 0;
    virtual void setLoginUrl(const QString &url) = 0;
    virtual QString loginUrl() const = 0;

    virtual void setUsername(const QString &uername) = 0;
    virtual QString username() const  = 0;

    virtual void setPassword(const QString &password) = 0;
    virtual QString password() const  = 0;

    virtual void setRequestData(const QByteArray &requestData) = 0;
    virtual QString requestData() const  = 0;

    virtual void setNetworkAccessManager(QNetworkAccessManager* nam) = 0;
    virtual QNetworkAccessManager*  networkAccessManager() const = 0;
    virtual void setNetworkCookieJar(NetworkCookieJar* cookieJar) = 0;
    virtual NetworkCookieJar* networkCookieJar() const = 0;

    virtual void setRawHeader(const QByteArray &headerName,
                      const QByteArray &headerValue) = 0;
    virtual QByteArray rawHeader() const = 0;

public slots:
    virtual void login() = 0;
    virtual void login(const QString &loginUrl,
                       const QString &username,
                       const QString &password) = 0;
    virtual void login(const QString &loginUrl,
                       const QString &username,
                       const QString &password,
                       const QByteArray &requestData) = 0;
private slots:
    virtual void finished(QNetworkReply* reply) = 0;

};

#endif // NETWORKLOGIN_H
