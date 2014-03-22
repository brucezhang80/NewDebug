#include "datatransfeoptions.h"
#include "ui_datatransfeoptions.h"

DataTransfeOptions::DataTransfeOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataTransfeOptions)
{
    ui->setupUi(this);
    connect( ui->pbtn_Commit, SIGNAL(clicked()), this, SLOT(startCommitSlot()) );
}

DataTransfeOptions::~DataTransfeOptions()
{
    delete ui;
}


void DataTransfeOptions::startCommitSlot()
{
    //ui->pbtn_Commit->setEnabled(false);
    emit startCommitSignal();
}

void DataTransfeOptions::updateControlSlot(bool enabled)
{
    QObjectList ctrlList = this->children();

    foreach(QObject *obj, ctrlList)
    {
        QWidget* widget = static_cast<QWidget*>(obj);
        if(widget)
        {
            widget->setEnabled(enabled);
        }
    }
}
