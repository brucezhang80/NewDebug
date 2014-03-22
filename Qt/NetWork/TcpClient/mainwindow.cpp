#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "clientsocket.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->serverAddrEdt->setText("192.168.1.2");
    ui->serverPortEdt->setText("4500");

    QString host = ui->serverAddrEdt->text();
    int port = ui->serverPortEdt->text().toInt();
    m_client = new ClientSocket(host, port);

}

MainWindow::~MainWindow()
{
    delete ui;
}
