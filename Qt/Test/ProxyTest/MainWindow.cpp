#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QAbstractItemView>

#include "Delegate.h"
#include "ProgressBar.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->addBtn, SIGNAL(clicked()), this, SLOT(addProgressbar()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addProgressbar()
{
//    for(int i = 0; i < 1000; i++)
    {
        ProgressBar *bar = new ProgressBar(this);
        ui->verticalLayout->addWidget(bar);

        Delegate *delegate = new Delegate;
        bar->setDelegate(delegate);
        bar->start();
    }

}
