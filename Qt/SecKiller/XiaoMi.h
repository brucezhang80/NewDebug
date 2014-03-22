/**************************************************************************
** Qt Creator license header template
**   Special keywords: Administrator 2013/12/11 2013
**   Environment variables:
**   To protect a percent sign, use '%'.
**************************************************************************/

#ifndef XIAOMI_H
#define XIAOMI_H

#include <QObject>
#include <QtNetwork>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QByteArray>

#include "NetWorkLogin.h"

class XiaoMi : public NetWorkLogin
{
    Q_OBJECT
public:
    explicit XiaoMi(QObject *parent = 0);
    XiaoMi(const QString &loginUrl,
           const QString &username,
           const QString &password,
           QObject *parent = 0);

    ~XiaoMi(){}
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

    void setHDInfoUrl(const QString &url) { m_hdInfoUrl = url; }
    QString hdInfoUrl() const { return m_hdInfoUrl; }

    QString hdInfoText();

    bool hasLogin();
    bool hasStarted();
    bool hasBuyUrl();

signals:
    void error(int errcode);

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
    QString                 m_hdInfoUrl;
    QString                 m_loginUrl;
    QString                 m_username;
    QString                 m_password;
    QByteArray              m_requestData;

    QByteArray              m_headerName;
    QByteArray              m_headerValue;

    bool                    m_bLogin;

    NetworkCookieJar*       m_cookieJar;
    QList<QNetworkCookie>   m_cookiesList;
    QNetworkAccessManager*  m_netManager;
};

#endif // XIAOMI_H
