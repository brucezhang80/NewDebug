#include "send_users_treewidget.h"

#include <QHeaderView>

SendUsersTreeWidget::SendUsersTreeWidget(QWidget *parent) :
    QTreeWidget(parent)
{
    initialize();
}

void SendUsersTreeWidget::retranslateUi()
{
    QStringList headers;
    headers << tr("Name") << tr("Ip");
    this->setHeaderLabels(headers);
    QFontMetrics fm = fontMetrics();
    QHeaderView *header = this->header();
    header->resizeSection(0, qMax(fm.width(headers.at(0)), fm.width(" login_name  ")) );
    header->resizeSection(1, qMax(fm.width(headers.at(1)), fm.width(" 00.00.00.00 ")));
}

void SendUsersTreeWidget::initialize()
{
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
//    setSortingEnabled(true);
    retranslateUi();
//    this->setItemDelegate(new SendUsersItemDelegate(this));

    completedPercent = 0;
}

int SendUsersTreeWidget::userCount() const
{
    return this->model()->rowCount();
}

int SendUsersTreeWidget::completedOfRow()
{
    return 0;
}
