/**************************************************************************
** Qt Creator license header template
**   Special keywords: Administrator 2013/12/13 2013
**   Environment variables:
**   To protect a percent sign, use '%'.
**************************************************************************/

#ifndef TAOBAOWIDGET_H
#define TAOBAOWIDGET_H

#include <QWidget>
#include "TaoBao.h"

class TaoBaoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TaoBaoWidget(QWidget *parent = 0);

    void setInfoString(const QString &infoString) { m_infoString = infoString;}
    QString showInfoString() const { return m_infoString;}

    void setUsername( const QString &username);
    void setPassword(const QString &password);

    QString url() const;

signals:
    void updateInfo(int type);
    void loginReady(int type);
    void loginReady(int type, const QNetworkRequest &request, const QByteArray &requestData);

public slots:
    void loginPost();
    void loginReady(const QNetworkRequest &request, const QByteArray &requestData);
    void loginError(int errcode);

private:
    TaoBao*     m_taobao;
    QString     m_infoString;
};

#endif // TAOBAOWIDGET_H
