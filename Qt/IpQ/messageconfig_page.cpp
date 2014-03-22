#include "messageconfig_page.h"
#include "ui_MessageConfigPage.h"

MessageConfigPage::MessageConfigPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MessageConfigPage)
{
    ui->setupUi(this);
}

MessageConfigPage::~MessageConfigPage()
{
    delete ui;
}

void MessageConfigPage::retranslateUi()
{
    ui->retranslateUi(this);
}
