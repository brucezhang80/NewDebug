
#include <QDateTime>
#include <QList>
#include <QDebug>

//#include "sidebar.h"
#include "jobsenderview.h"
#include "jobhistoryview.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initMainWindow();

    m_sidebarBtnMapping = new QSignalMapper(this);
    connect(m_sidebarBtnMapping, SIGNAL(mapped(int)), ui->stackedWidget, SLOT(setCurrentIndex(int)));
    setSidebarBtnMap();

    connect(ui->jobSenderView, SIGNAL(updateSenderBtn(bool)), this, SLOT(updateSenderBtn(bool)));
    connect(ui->btn_senSend, SIGNAL(clicked()), ui->jobSenderView, SLOT(startUploadDataClientSlot()));

    connect(ui->jobSenderView, SIGNAL(saveToHistoryView(const QString&, const QString&)),
            ui->jobHistoryView, SLOT(saveSenderData(const QString&, const QString&)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::initMainWindow()
{
    ui->sideBar->setFixedWidth(90);

    ui->stackedWidget->setCurrentIndex(0);

    //0
    ui->splitter_sen->setStretchFactor(0, 75);
    ui->splitter_sen->setStretchFactor(1, 25);
    ui->splitter_sen->setContentsMargins(0, 0, 0, 0);

    //2
    ui->splitter_his->setStretchFactor(0, 75);
    ui->splitter_his->setStretchFactor(1, 25);
    ui->splitter_his->setContentsMargins(0, 0, 0, 0);

    // 5
    ui->splitter_per->setStretchFactor(0, 75);
    ui->splitter_per->setStretchFactor(1, 25);
    ui->splitter_per->setContentsMargins(0, 0, 0, 0);

    initStatusbar();
    return true;
}

void MainWindow::initStatusbar()
{
//    QProgressBar *progressBar = new QProgressBar();
    QLabel *mode = new QLabel( tr(" Ready ") );
    QLabel *number = new QLabel( tr(" Counts: ") );
    timeStatus = new QLabel( tr("  Time: 0000/00/00 00:00:00") );

    mode->setMinimumSize( mode->sizeHint() );
    mode->setAlignment( Qt::AlignCenter );
    mode->setText( tr("Ready") );
    mode->setToolTip( tr("The current working mode.") );

    number->setMinimumSize( number->sizeHint() );
    number->setAlignment( Qt::AlignCenter );
    number->setText( tr(" 0 ") );
    number->setToolTip( tr("Current document count(s).") );

    QTimer * timer = new QTimer();
    int sec = 1;
    QObject::connect(timer,SIGNAL(timeout()), this, SLOT(showStatusbarTime()));
    timer->start(sec * 1000);

    timeStatus->setMinimumSize( timeStatus->sizeHint() );
    timeStatus->setAlignment( Qt::AlignRight | Qt::AlignVCenter );
    timeStatus->setToolTip( tr("Current system time.") );

//    progressBar->setTextVisible( false );
//    progressBar->setRange( 0, 0 );
    ui->statusBar->addWidget( mode );
    ui->statusBar->addWidget( number );
    //ui->statusBar->addWidget( time );
    ui->statusBar->addPermanentWidget( timeStatus );////现实永久信息
    //ui->statusBar->addWidget( progressBar, 1 );

//    ui->statusBar->showMessage( tr("Ready"), 2000 );

}

void MainWindow::showStatusbarTime()
{
    QDateTime dt;
    QTime curTime;
    QDate curData;
    dt.setDate(curData.currentDate());
    dt.setTime(curTime.currentTime());
    QString currentDate = dt.toString("yyyy/MM/dd HH:mm:ss");

    timeStatus->setText(" Time: "+ currentDate );
}

// sender done update button enabled or disabled.
void MainWindow::updateSenderBtn(bool isEnabled)
{
    QList<QPushButton *> allButtons = ui->topBar->findChildren<QPushButton *>();
    foreach( QPushButton* obj, allButtons)
    {
        if( obj->isEnabled())
            isEnabled = !isEnabled;

        obj->setEnabled(!isEnabled);
    }
}

void MainWindow::setSidebarBtnMap()
{
    QList<QToolButton *> allButtons = ui->sideBar->findChildren<QToolButton *>();
    int index=0;
    foreach( QToolButton* obj, allButtons)
    {
        connect(obj, SIGNAL(clicked()), m_sidebarBtnMapping, SLOT(map()));
        m_sidebarBtnMapping->setMapping(obj, index);
        index++;
    }
}

