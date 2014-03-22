/**************************************************************************
** Qt Creator license header template
**   Special keywords: Administrator 2013/12/12 2013
**   Environment variables:
**   To protect a percent sign, use '%'.
**************************************************************************/

#include "MusicItemWidget.h"
#include "ui_MusicItemWidget.h"

MusicItemWidget::MusicItemWidget(QWidget *parent):
    QWidget(parent),
    ui(new Ui::MusicItemWidget)
{
    ui->setupUi(this);
}

MusicItemWidget::MusicItemWidget(QListWidgetItem* item, QWidget *parent) :
    QWidget(parent),
    m_item(item),
    ui(new Ui::MusicItemWidget)
{
    ui->setupUi(this);
}

MusicItemWidget::~MusicItemWidget()
{
    delete ui;
}

void MusicItemWidget::leaveEvent(QEvent *event)
{
    emit mouseLeave(m_item);
    QWidget::leaveEvent(event);
}
