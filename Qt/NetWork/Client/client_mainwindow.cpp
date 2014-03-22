#include "client_mainwindow.h"
#include "ui_client_mainwindow.h"
#include "constance.h"
#include "client.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle( APP_TITLE );

    m_client = new Client(this);
    ui->localIP->setText( m_client->address() );

    connect( ui->connectBtn, SIGNAL(toggled(bool)), this, SLOT(connectToServer(bool)) );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectToServer(bool on)
{
    if( on )
    {
        ui->connectBtn->setText( tr("Disconnet") );
        QString serverIP = ui->serverIP->text();
        int serverPort = ui->serverPort->text().toInt();

        m_client->connectToServer(serverIP, serverPort);
    }
    else
    {
        qDebug() << "Disconnected server.";

        ui->connectBtn->setText( tr("Connect") );
        m_client->close();
    }
}
