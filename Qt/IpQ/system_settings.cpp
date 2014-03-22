#include "system_settings.h"
#include "ui_systemsettings.h"

#include "configuration_page.h"
#include "messageconfig_page.h"
#include "folderconfig_page.h"
#include "helperconfig_page.h"
#include "macai.h"

#include <QDebug>

SystemSettings::SystemSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SystemSettings)
{
    ui->setupUi(this);

    ui->contentsWidget->setViewMode(QListView::IconMode);
    ui->contentsWidget->setIconSize(QSize(78, 72));
    ui->contentsWidget->setMovement(QListView::Static);
    ui->contentsWidget->setMinimumWidth(128);
    ui->contentsWidget->setMaximumWidth(128);
    ui->contentsWidget->setSpacing(12);

    ui->pagesWidget->addWidget(Macai::configurationPage);
    ui->pagesWidget->addWidget(Macai::messageConfigPage);
    ui->pagesWidget->addWidget(Macai::folderConfigPage);
    ui->pagesWidget->addWidget(Macai::helperConfigPage);

    createIcons();
    ui->contentsWidget->setCurrentRow(0);

//    setFixedSize(570, 400);
    setWindowTitle(tr("System Config"));
}

SystemSettings::~SystemSettings()
{
    delete ui;
}

void SystemSettings::retranslateUi()
{
    ui->retranslateUi(this);
}

void SystemSettings::createIcons()
{
    QListWidgetItem *configButton = new QListWidgetItem(ui->contentsWidget);
    configButton->setIcon(QIcon(":/images/system-config.png"));
    configButton->setText(tr("Configuration"));
    configButton->setTextAlignment(Qt::AlignHCenter);
    configButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *messageButton = new QListWidgetItem(ui->contentsWidget);
    messageButton->setIcon(QIcon(":/images/system-message.png"));
    messageButton->setText(tr("Message"));
    messageButton->setTextAlignment(Qt::AlignHCenter);
    messageButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *folderButton = new QListWidgetItem(ui->contentsWidget);
    folderButton->setIcon(QIcon(":/images/system-folder-config.png"));
    folderButton->setText(tr("Folder"));
    folderButton->setTextAlignment(Qt::AlignHCenter);
    folderButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *helpButton = new QListWidgetItem(ui->contentsWidget);
    helpButton->setIcon(QIcon(":/images/system-help.png"));
    helpButton->setText(tr("Help"));
    helpButton->setTextAlignment(Qt::AlignHCenter);
    helpButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    connect(ui->contentsWidget,
            SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(changePage(QListWidgetItem*,QListWidgetItem*)));
}

void SystemSettings::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
        current = previous;

    ui->pagesWidget->setCurrentIndex(ui->contentsWidget->row(current));
}















