#include "folderconfig_page.h"
#include "ui_folderconfigpage.h"
#include "system.h"

FolderConfigPage::FolderConfigPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FolderConfigPage)
{
    ui->setupUi(this);
    initialize();
}

FolderConfigPage::~FolderConfigPage()
{
    delete ui;
}

void FolderConfigPage::retranslateUi()
{
    ui->retranslateUi(this);
}

void FolderConfigPage::initialize()
{
    QString app_path = System::appPath();
    QString recv_path = System::sharedPath();
    QString log_path = System::localIPQDocument();
    ui->appPathEdit->setText(app_path);
    ui->recievePathEdit->setText(recv_path);
    ui->logPathEdit->setText(log_path);
}
