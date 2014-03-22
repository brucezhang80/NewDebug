/**************************************************************************
** Qt Creator license header template
**   Special keywords: Administrator 2013/12/13 2013
**   Environment variables:
**   To protect a percent sign, use '%'.
**************************************************************************/

#ifndef XIAOMIWIDGET_H
#define XIAOMIWIDGET_H

#include <QWidget>
#include "XiaoMi.h"

class XiaoMiWidget : public QWidget
{
    Q_OBJECT
public:
    explicit XiaoMiWidget(QWidget *parent = 0);

    void setInfoString(const QString &infoString) { m_infoString = infoString;}
    QString showInfoString() const { return m_infoString;}

    void setUsername( const QString &username);
    void setPassword(const QString &password);

signals:
    void updateInfo(int type);

public slots:
    void loginPost();
    void loginError(int errcode);

    bool isLogin();

private:
    XiaoMi*     m_xiaomi;
    QString     m_infoString;
    QString     m_xmBuyInfo;
};

#endif // XIAOMIWIDGET_H
