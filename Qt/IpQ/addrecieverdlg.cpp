#include "addrecieverdlg.h"
#include "macai.h"
#include "usertreewidget.h"
#include "useritemwidget.h"
#include "transfer_file_window.h"
#include <QStringList>
#include <QTreeWidgetItemIterator>

#include <QDebug>

AddRecieverDlg::AddRecieverDlg(QWidget *parent) :
    QDialog(parent),ui(new Ui::AddRecieverDlg)
{
    ui->setupUi(this);
    initialize();

    connect(ui->addBtn, SIGNAL(clicked()), this, SLOT(add()));
    connect(ui->delBtn, SIGNAL(clicked()), this, SLOT(del()));
    connect(ui->okBtn, SIGNAL(clicked()), this, SLOT(ok()));
}

void AddRecieverDlg::retranslateUi()
{
    ui->retranslateUi(this);
}

void AddRecieverDlg::initialize()
{
    m_userInfoMap.clear();
    QStringList list;
    QTreeWidgetItemIterator it(Macai::userTreeWidget);
    while (*it)
    {
        QWidget* widget = Macai::userTreeWidget->itemWidget(*it, 0);

        if( widget && widget->objectName() == QString("UserItemWidget") )
        {
            UserItemWidget* userwidget = static_cast<UserItemWidget*>(widget);
            if( userwidget )
            {
                QString name = userwidget->loginName();
                QString ip = userwidget->userIp();
                QString label = name + ":" + ip;
                list.append(label);
                m_userInfoMap.insert(name, ip);
            }
        }
        ++it;
   }
    ui->allUserlistWidget->addItems(list);
}

void AddRecieverDlg::add()
{
    QList<QListWidgetItem*> itemList = ui->allUserlistWidget->selectedItems();
    foreach( QListWidgetItem *item, itemList)
    {
        ui->newUserlistWidget->addItem(item->text());
        delete item;
    }
}

void AddRecieverDlg::del()
{
   QList<QListWidgetItem*> itemList = ui->newUserlistWidget->selectedItems();
   foreach( QListWidgetItem *item, itemList)
   {
       ui->allUserlistWidget->addItem(item->text());
       delete item;
   }
}

void AddRecieverDlg::ok()
{
    int count = ui->newUserlistWidget->count();
    if( count != 0)
    {
        QList<QListWidgetItem*> itemList = ui->newUserlistWidget->selectedItems();

        for(int i = 0; i < count; ++i)
        {
            QListWidgetItem *item = ui->newUserlistWidget->item(i);
            QString label = item->text();
            QStringList list = label.split( ":");
            Macai::transferFileWindow->insertUserToList(list.at(0), list.at(1));
        }
    }
    this->accept();
}

