/**************************************************************************
** Qt Creator license header template
**   Special keywords: Administrator 2013/12/13 2013
**   Environment variables:
**   To protect a percent sign, use '%'.
**************************************************************************/

#include "TaoBaoWidget.h"
#include "MainWindow.h"

#include <QDebug>

TaoBaoWidget::TaoBaoWidget(QWidget *parent) :
    QWidget(parent)
{
        m_taobao = new TaoBao;

        connect(m_taobao, SIGNAL(error(int)), this, SLOT(loginError(int)) );
        connect(m_taobao, SIGNAL(loginReady(QNetworkRequest, QByteArray)),
                this, SLOT(loginReady(QNetworkRequest, QByteArray)) );

}

void TaoBaoWidget::setUsername(const QString &username)
{
    m_taobao->setUsername(username);
}

void TaoBaoWidget::setPassword(const QString &password)
{
    m_taobao->setPassword(password);
}

QString TaoBaoWidget::url() const
{
    return m_taobao->loginUrl();
}

void TaoBaoWidget::loginPost()
{
    QByteArray requestData = QString("TPL_username=%1&TPL_password=%2"
                            "&actionForStable=enable_post_user_action&action=Authenticator&event_submit_do_login=anything")
                            .arg(m_taobao->username()).arg(m_taobao->password()).toLocal8Bit();

    qDebug() << requestData;

    m_taobao->setRequestData(requestData);
    m_taobao->login();
}

void TaoBaoWidget::loginReady(const QNetworkRequest &request, const QByteArray &requestData)
{
    emit loginReady(1, request, requestData);
}

void TaoBaoWidget::loginError(int errcode)
{
    if(errcode == QNetworkReply::NoError)
    {
        m_infoString = tr("ÌÔ±¦µÇÂ¼³É¹¦");
    }
    else
    {
        m_infoString = tr("ÌÔ±¦µÇÂ¼Ê§°Ü");
    }

    emit updateInfo(1);
}
