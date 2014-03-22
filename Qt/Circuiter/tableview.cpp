#include "tableview.h"

TableView::TableView(QWidget *parent) :
    QTableView(parent)
{
    this->m_model = new QStandardItemModel(32, 4, this);

    initTableView();
}

void TableView::initTableView()
{
    createHeader();
}

void TableView::createHeader()
{
    m_model->setHorizontalHeaderItem(0, new QStandardItem(QString("Name")));
    m_model->setHorizontalHeaderItem(1, new QStandardItem(QString("Date")));
    m_model->setHorizontalHeaderItem(2, new QStandardItem(QString("Size")));
    m_model->setHorizontalHeaderItem(3, new QStandardItem(QString("User")));
    m_model->setHorizontalHeaderItem(4, new QStandardItem(QString("State")));
    m_model->setHorizontalHeaderItem(5, new QStandardItem(QString("Progress")));

    this->setModel(m_model);
}
