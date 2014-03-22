#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "serversocket.h"
#include <QList>
#include <QStringList>
#include <QHostAddress>
#include <QNetworkInterface>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setServerHost();

    ui->serverPortEdt->setText("4500");
    int port = ui->serverPortEdt->text().toInt();
    m_server = new ServerSocket(this);
    m_server->setServerPort(port);

    m_server->listen(QHostAddress::Any, port);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setServerHost()
{
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i)
    {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
                ipAddressesList.at(i).toIPv4Address())
        {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    ui->serverAddrEdt->setText(ipAddress);

}
