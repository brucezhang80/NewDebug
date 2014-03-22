#include "helperconfig_page.h"
#include "ui_helperconfigpage.h"

HelperConfigPage::HelperConfigPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HelperConfigPage)
{
    ui->setupUi(this);
    ui->splitter->setStretchFactor(0, 20);
    ui->splitter->setStretchFactor(1, 80);

}

HelperConfigPage::~HelperConfigPage()
{
    delete ui;
}

void HelperConfigPage::retranslateUi()
{
    ui->retranslateUi(this);
}
