/**************************************************************************
** Qt Creator license header template
**   Special keywords: Administrator 2013/12/13 2013
**   Environment variables:
**   To protect a percent sign, use '%'.
**************************************************************************/

#include "XiaoMiWidget.h"

XiaoMiWidget::XiaoMiWidget(QWidget *parent) :
    QWidget(parent)
{
    m_xiaomi = new XiaoMi;
    connect(m_xiaomi, SIGNAL(error(int)), this, SLOT(loginError(int)) );
}

void XiaoMiWidget::setUsername(const QString &username)
{
    m_xiaomi->setUsername(username);
}

void XiaoMiWidget::setPassword(const QString &password)
{
    m_xiaomi->setPassword(password);
}

void XiaoMiWidget::loginPost()
{
    QByteArray requestData = QString("passToken=&user=" + m_xiaomi->username() +
                                     "&_json=true&pwd=" + m_xiaomi->password() +
                                     "&sid=passport&_sign=KKkRvCpZoDC%2BgLdeyOsdMhwV0Xg%3D"
                                     "&callback=https%3A%2F%2Faccount.xiaomi.com"
                                     "&qs=%253Fsid%253Dpassport&hidden=")
                                     .toLocal8Bit();

    m_xiaomi->setRequestData(requestData);
    m_xiaomi->login();
}

void XiaoMiWidget::loginError(int errcode)
{
    if(errcode == QNetworkReply::NoError && m_xiaomi->hasLogin())
    {
        m_infoString = tr("小米登录成功");
    }
    else
    {
        m_infoString = tr("小米登录失败");
    }

    emit updateInfo(2);
}

bool XiaoMiWidget::isLogin()
{

    return m_xiaomi->hasLogin();
}
