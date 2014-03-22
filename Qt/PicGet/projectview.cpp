#include "projectview.h"
#include "treemodel.h"


ProjectView::ProjectView(QWidget *parent) :
    QTreeWidget(parent)
{
    this->setColumnCount(1);
    QStringList headers;
    headers << tr("״̬") << tr("����");
    this->setHeaderLabels(headers);

    QFontMetrics fm = fontMetrics();
    QHeaderView *header = this->header();
    header->resizeSection(0, fm.width(headers.at(0)+"  "));
    header->resizeSection(1, qMax(fm.width(headers.at(1)), fm.width(" ")) );

    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setAlternatingRowColors(true);
    this->setRootIsDecorated(false);
    this->setSortingEnabled(true);
    this->sortByColumn(1, Qt::AscendingOrder);
    this->setSortingEnabled(1);

    this->setStyleSheet( "QTreeWidget::item:hover{background-color:rgb(0,255,0,50)}"
    "QTreeWidget::item:selected{background-color:rgb(255,0,0,100)}");

}

void ProjectView::addProject(PicProject *picPro)
{
    QList<QTreeWidgetItem *> rootList;
    QTreeWidgetItem* proTitle = new QTreeWidgetItem(this);
    proTitle->setText( 0, QString("%1").arg(picPro->m_proId) );
    proTitle->setText( 1, picPro->m_proTitle );
    rootList.append(proTitle);

    QTreeWidgetItem *pathItem = new QTreeWidgetItem(proTitle); //����ӽڵ�
    pathItem->setText(1, picPro->m_proPath);
    proTitle->addChild(pathItem);

    QTreeWidgetItem *urlItem = new QTreeWidgetItem(proTitle); //����ӽڵ�
    urlItem->setText(1, picPro->m_proUrl);
    proTitle->addChild(urlItem);

    this->insertTopLevelItems(this->model()->rowCount(), rootList);  //�������벿����
    this->collapse(this->currentIndex());
}
