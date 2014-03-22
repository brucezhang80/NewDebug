#include "groupwidget.h"
#include "ui_groupwidget.h"
#include "friendstree.h"

#include <QDebug>

GroupWidget::GroupWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupWidget)
{
    ui->setupUi(this);
    connect(ui->btn_sendFiles, SIGNAL(clicked()), this, SLOT(sendFiles()) );
}

GroupWidget::~GroupWidget()
{
    delete ui;
}

void GroupWidget::setGroupName(const QString &groupname)
{
    ui->lb_groupName->setText(groupname);
    m_groupName = ui->lb_groupName->text();
}

QString GroupWidget::groupName() const
{
    return m_groupName;
}

void GroupWidget::setGroupItem(QTreeWidgetItem *item)
{
    m_groupItem = item;
}

QTreeWidgetItem *GroupWidget::groupItem() const
{
    return m_groupItem;
}

void GroupWidget::sendFiles()
{
    emit showSendFileWidget();
}

void GroupWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
//    FriendsTree* tree = static_cast<FriendsTree*>(this->window());
//    qDebug() << tree;
//    if( tree )
//    {

//        QTreeWidgetItem* item =  tree->itemAt(e->pos());

//        if(!item) return;

//        qDebug() << item->text(0);

//        if( item->isExpanded() )
//            item->setExpanded(false);
//        else
//            item->setExpanded(true);


//    }
    return;
}
