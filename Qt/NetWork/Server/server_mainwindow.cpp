#include "server_mainwindow.h"
#include "ui_server_mainwindow.h"
#include "server.h"
#include "constance.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle( APP_TITLE );
    m_server = new Server(this);
    ui->serverIP->setText( m_server->host() );
    ui->serverPort->setText( QString("%1").arg( m_server->port()) );

    connect( ui->startBtn, SIGNAL(toggled(bool)), this, SLOT(onStartServer(bool)) );
}

MainWindow::~MainWindow()
{
    m_server->deleteLater();
    delete ui;
}

void MainWindow::onStartServer(bool on)
{
    if( on )
    {
        ui->startBtn->setText( tr("Stop") );
        bool ok = m_server->listen();
        if( !ok )
        {
            QMessageBox::critical(this, APP_TITLE,
                              tr("Unable to start the server: %1.")
                              .arg(m_server->errorString()));
        }
    }
    else
    {
        ui->startBtn->setText( tr("Start") );
        m_server->close();
    }
}
