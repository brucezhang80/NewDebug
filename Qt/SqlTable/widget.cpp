#include "widget.h"
#include "ui_widget.h"
#include <QItemDelegate>
#include <QHeaderView>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    // 设置垂直header的第0行的宽度
    QHeaderView *vHeader = ui->tableView->verticalHeader();
    vHeader->resizeSection(0, 100);

    // 设置水平header的第0列的宽度
    QHeaderView *hHeader = ui->tableView->horizontalHeader();
    //hHeader->resizeSection(0, 200);
    hHeader->resizeMode(QHeaderView::ResizeToContents);
}

Widget::~Widget()
{
    delete ui;
}
