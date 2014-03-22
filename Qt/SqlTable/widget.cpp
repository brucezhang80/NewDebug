#include "widget.h"
#include "ui_widget.h"
#include <QItemDelegate>
#include <QHeaderView>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    // ���ô�ֱheader�ĵ�0�еĿ��
    QHeaderView *vHeader = ui->tableView->verticalHeader();
    vHeader->resizeSection(0, 100);

    // ����ˮƽheader�ĵ�0�еĿ��
    QHeaderView *hHeader = ui->tableView->horizontalHeader();
    //hHeader->resizeSection(0, 200);
    hHeader->resizeMode(QHeaderView::ResizeToContents);
}

Widget::~Widget()
{
    delete ui;
}
