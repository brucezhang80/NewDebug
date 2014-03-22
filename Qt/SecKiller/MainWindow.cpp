/**************************************************************************
** Qt Creator license header template
**   Special keywords: Administrator 2013/12/6 2013
**   Environment variables:
**   To protect a percent sign, use '%'.
**************************************************************************/

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "TaoBaoWidget.h"
#include "XiaoMiWidget.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->splitter->setStretchFactor(0, 30);
    ui->splitter->setStretchFactor(1, 70);

    ui->tabWidget->setCurrentIndex(0);

    ui->tb_usernameEdit->setText("yvi¿àÂé");
    ui->tb_passwordEdit->setText("setPsw(\"tb\");");
    ui->taobaoTab->setUsername(ui->tb_usernameEdit->text());
    ui->taobaoTab->setPassword(ui->tb_passwordEdit->text());

    ui->xm_usernameEdit->setText("12319597@qq.com");
    ui->xm_passwordEdit->setText("setpsw(\"xm\");");
    ui->xiaomiTab->setUsername(ui->xm_usernameEdit->text());
    ui->xiaomiTab->setPassword(ui->xm_passwordEdit->text());

    connect(ui->tb_loginBtn, SIGNAL(clicked()), ui->taobaoTab, SLOT(loginPost()) );
    connect(ui->taobaoTab, SIGNAL(updateInfo(int)), this, SLOT(updateInfo(int)) );
    connect(ui->taobaoTab, SIGNAL(loginReady(int,QNetworkRequest, QByteArray)),
            this, SLOT(loginFinished(int, QNetworkRequest, QByteArray)) );

    connect(ui->xm_loginBtn, SIGNAL(clicked()), ui->xiaomiTab, SLOT(loginPost()) );
    connect(ui->xiaomiTab, SIGNAL(updateInfo(int)), this, SLOT(updateInfo(int)) );
    connect(ui->xm_panicBuyBtn, SIGNAL(clicked()), this, SLOT(startPanicBuying()) );

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateInfo(int type)
{
    switch(type)
    {
    case 1:
        ui->statusBar->showMessage(ui->taobaoTab->showInfoString());
        break;
    case 2:
        ui->statusBar->showMessage(ui->xiaomiTab->showInfoString());
        break;
    default:return;
    }
}

void MainWindow::loginFinished(int type, const QNetworkRequest &request,
                               const QByteArray &requestData)
{
    switch(type)
    {
    case 1:
    {
        QString url = ui->taobaoTab->url();
//        ui->webView->load( request, QNetworkAccessManager::PostOperation, requestData );

//        ui->urlEdit->setText(request.url().toString());
    };
        break;
    case 2:
        if( ui->xiaomiTab->isLogin())
            ui->xm_panicBuyBtn->setEnabled(true);
        break;
    default:return;
    }
}

void MainWindow::updateBtn(bool enabled)
{
    ui->xm_panicBuyBtn->setEnabled(enabled);
    ui->xm_loginBtn->setEnabled(!enabled);
    if( ui->xm_panicBuyBtn->isEnabled() )
        ui->xm_stopBuyBtn->setEnabled(!ui->xm_panicBuyBtn->isEnabled());
}

void MainWindow::startPanicBuying()
{

}
