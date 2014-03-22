#include "tcptransfer.h"
#include "ui_tcptransfer.h"
#include "friendsview.h"
#include "filesview.h"
#include "historyupload.h"

TCPTransfer::TCPTransfer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TCPTransfer)
{
    ui->setupUi(this);
    ui->friendsTBtn->setCheckable(1);
    ui->uploadTBtn->setCheckable(1);
    ui->historyTBtn->setCheckable(1);
    ui->friendsTBtn->setChecked(true);

    ui->addPBtn->setEnabled(false);
    ui->startUploadTBtn->setEnabled(false);
    ui->stopUploadTBtn->setEnabled(false);
    ui->clearTaskPBtn->setEnabled(false);

    m_friendsView = new FriendsView(this);
    m_filesView = new FilesView(this);
    m_historyUpload = new HistoryUpload(this);
    ui->stackedWidget->insertWidget(0, m_friendsView);
    ui->stackedWidget->insertWidget(1, m_filesView);
    ui->stackedWidget->insertWidget(2, m_historyUpload);

    ui->stackedWidget->setCurrentIndex(0);


    bUploading = false;
    serverName = "//VM_XP/";
    serverPubRoot = serverName+ "PubProjects/";

    QStringList listName;
    listName << tr("XX项目0") << tr("XX项目1") << tr("XX项目2") << tr("XX项目3") << tr("XX项目4");
    ui->projectsCbox->addItems(listName);
    ui->projectsCbox->setHidden(true);

    connect( ui->friendsTBtn, SIGNAL(clicked()), this, SLOT(setCurrentStackWidgetSlot()) );
    connect( ui->uploadTBtn, SIGNAL(clicked()), this, SLOT(setCurrentStackWidgetSlot()) );
    connect( ui->historyTBtn, SIGNAL(clicked()), this, SLOT(setCurrentStackWidgetSlot()) );
    connect( this, SIGNAL(currentStackWidgetSingal(int)), this->ui->stackedWidget, SLOT(setCurrentIndex(int)) );

    connect( ui->startUploadTBtn, SIGNAL(clicked()), m_filesView, SLOT(startUploadDataClientSlot()) );
    connect( ui->clearTaskPBtn, SIGNAL(clicked()), m_filesView, SLOT(clearAllClient()) );
    connect( ui->stopUploadTBtn, SIGNAL(clicked()), m_filesView, SLOT(stopUploadDataClientSlot()) );
    connect( ui->projectsCbox, SIGNAL(currentIndexChanged(int)), this, SLOT(currentComboboxIndex(int)) );

    connect( m_filesView, SIGNAL(updateControlSignal(bool)), this, SLOT(updateControl(bool)) );
}

TCPTransfer::~TCPTransfer()
{
    delete ui;
}

void TCPTransfer::setCurrentStackWidgetSlot()
{
    QObject* sender = this->sender();
    QToolButton* tbtn = static_cast<QToolButton*>(sender);
    if(!tbtn) return;

    ui->friendsTBtn->setChecked(false);
    ui->uploadTBtn->setChecked(false);
    ui->historyTBtn->setChecked(false);

    int index = 0;
    if( tbtn == ui->friendsTBtn )
    {
        tbtn->setChecked(true);
        index = 0;
    }
    else if( tbtn == ui->uploadTBtn )
    {
        tbtn->setChecked(true);
        index = 1;
    }
    else if( tbtn == ui->historyTBtn )
    {
        tbtn->setChecked(true);
        index = 2;
    }

    updateButton(index);
    emit currentStackWidgetSingal(index);
}

void TCPTransfer::updateControl(bool bEnabled)
{
    bUploading = !bEnabled;
    ui->startUploadTBtn->setEnabled(bEnabled);
    ui->clearTaskPBtn->setEnabled(bEnabled);
    ui->projectsCbox->setEnabled(bEnabled);
    ui->stopUploadTBtn->setEnabled(bUploading);
}

void TCPTransfer::currentComboboxIndex(int index)
{
    QStringList list = getProjectPathList();
    QString path = list.at(index);
    //emit currentProjectPath(path);
    m_filesView->setServerPath(path);
}

void TCPTransfer::updateButton(int index)
{
    if( bUploading) return;
    switch( index )
    {
    case 0:
        ui->addPBtn->setEnabled(false);
        ui->startUploadTBtn->setEnabled(false);
        ui->stopUploadTBtn->setEnabled(false);
        ui->clearTaskPBtn->setEnabled(false);
        ui->projectsCbox->setHidden(true);
        break;
    case 1:
    {
        ui->addPBtn->setEnabled(true);
        ui->startUploadTBtn->setEnabled(true);
        ui->stopUploadTBtn->setEnabled(false);
        ui->clearTaskPBtn->setEnabled(true);
        ui->projectsCbox->setHidden(false);

        int index = ui->projectsCbox->currentIndex();
        QStringList list = getProjectPathList();
        QString path = list.at(index);
        m_filesView->setServerPath(path);
    };
        break;
    case 2:
        ui->addPBtn->setEnabled(false);
        ui->startUploadTBtn->setEnabled(false);
        ui->stopUploadTBtn->setEnabled(false);
        ui->clearTaskPBtn->setEnabled(false);
        ui->projectsCbox->setHidden(true);
        break;
    default:
        return;
    }
}

QStringList TCPTransfer::getProjectPathList() const
{
    QStringList listPath;
    listPath << serverPubRoot + "XX项目0/" << serverPubRoot + "XX项目1/"
             << serverPubRoot + "XX项目2/" << serverPubRoot + "XX项目3/"<< serverPubRoot + "XX项目4/";

    return listPath;
}
