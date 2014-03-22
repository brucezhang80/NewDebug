/**************************************************************************
** Qt Creator license header template
**   Special keywords: Administrator 2013/12/11 2013
**   Environment variables:
**   To protect a percent sign, use '%'.
**************************************************************************/

#ifndef NETWORKCOOKIES_H
#define NETWORKCOOKIES_H

#include <QNetworkCookieJar>

class NetworkCookieJar : public QNetworkCookieJar
{
    Q_OBJECT
public:
    explicit NetworkCookieJar(QObject *parent = 0);
    QList<QNetworkCookie> cookies() { return allCookies(); }

signals:

public slots:

};

#endif // NETWORKCOOKIES_H
