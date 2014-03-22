#include "pictureview.h"

PictureView::PictureView(QWidget *parent) :
    QTreeWidget(parent)
{
    QStringList headers;
    headers << tr("序号") << tr("图片") << tr("进度")
            << tr("下载速度") << tr("下载状态") << tr("操作");

    this->setHeaderLabels(headers);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setAlternatingRowColors(true);
    this->setRootIsDecorated(false);
    this->setSortingEnabled(true);
    this->sortByColumn(0, Qt::AscendingOrder);
    // Set header resize modes and initial section sizes
    QFontMetrics fm = fontMetrics();
    QHeaderView *header = this->header();
    header->resizeSection(0, 20);
    header->resizeSection(1, fm.width("name-for-a-picture.jpg"));
    header->resizeSection(2, fm.width(headers.at(2) + "========"));
    header->resizeSection(4, qMax(fm.width(headers.at(3) + "  "), fm.width(" 1234.0 KB/s ")));
    header->resizeSection(4, qMax(fm.width(headers.at(4) + "  "), fm.width(tr("正在下载") + "  ")));
    //header->resizeSection(5, 20);

    this->setItemDelegate(new DataViewDelegate(parent));

    this->setStyleSheet( "QTreeWidget::item:hover{background-color:rgb(0,255,0,50)}"
    "QTreeWidget::item:selected{background-color:rgb(255,0,0,100)}");

    addPicture();
}

void PictureView::addPicture()
{
    for(int i = 0; i < 10; ++i)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(this);
        item->setText(0, QString("%1").arg(i));
        item->setText(1, QString("Unknow Name %1").arg(i));
        item->setText(2, QString("%1.0 KB/s").arg(i));
        item->setText(3, QString("%1.0 KB").arg(i));
        item->setText(4, tr("下载失败"));
    }
}
