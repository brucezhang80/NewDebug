#include "table_widget.h"

#include <QHeaderView>
#include <QToolButton>
#include <QPushButton>
#include <QStringList>
#include <QGridLayout>


#include <QDebug>

TableWidget::TableWidget(QWidget *parent) :
    QTableWidget(parent)
{
    initialize();
}

void TableWidget::initialize()
{
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);// 禁止编辑
    this->verticalHeader()->setVisible(false); //隐藏列表头
    this->horizontalHeader()->setVisible(false); //隐藏行表头

//    this->setDragEnabled(true);
//    this->setDragDropMode(QAbstractItemView::InternalMove);
//    this->setShowGrid(false);

    this->setFocusPolicy(Qt::ClickFocus);
    this->setFrameShape(QFrame::NoFrame);
    this->setFrameShadow(QFrame::Sunken);
    this->setLineWidth(3);
    this->setCornerButtonEnabled(false);
    this->setAlternatingRowColors(false);
    this->horizontalHeader()->setDefaultSectionSize(180);
    this->horizontalHeader()->setStretchLastSection(true);
    this->verticalHeader()->setStretchLastSection(false);

    this->setColumnCount(1);
    this->initStyleSheet();
}

void TableWidget::initStyleSheet()
{
    this->setObjectName(QString::fromUtf8("tableWidget"));
    this->setStyleSheet(QString::fromUtf8("QToolButton:hover\n"
                                           "{\n"
                                           "	background-color: rgb(0, 117, 171);\n"
                                           "	color: rgb(0, 0, 0);\n"
                                           "	border-color: rgb(255, 255, 255);\n"
                                           "	border-radius: 6px;\n"
                                           " 	border: 4px;\n"
                                           "}\n"
                                           "QToolButton\n"
                                           "{ /* all types of tool button */\n"
                                           "	 border: 2px;\n"
                                           "     border-radius: 6px;\n"
                                           " }\n"
                                           "QToolButton[popupMode=\"1\"]\n"
                                           "{ \n"
                                           "/* only for MenuButtonPopup */\n"
                                           "     padding-right: 20px; /* make way for the popup button */\n"
                                           "}\n"
                                           "QToolButton:pressed\n"
                                           "{\n"
                                           "	background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(130, 200, 250, 255), stop:1 rgba(130, 200, 250, 255));\n"
                                          "}"));
}

QList<QToolButton*> TableWidget::addButton(const int &count, const QStringList &titles, const QList<QIcon> &icons)
{
    QList<QToolButton*> btnList;

    return btnList;
}

QList<QToolButton*> TableWidget::addButton(const int &count, const QStringList &titles)
{
    QList<QToolButton*> btnList;
    int begin = rowCount();
    int total = begin + count;
    this->setRowCount(total);
    for(int i = begin; i < total; ++i )
    {
        int j = i-begin;
        QToolButton *toolbtn = new QToolButton(this);
        toolbtn->setText( titles.at(j) );
        this->setCellWidget(i, 0, toolbtn);
        btnList.append(toolbtn);
    }

    return btnList;
}

QToolButton* TableWidget::addButton(const QString &title, const QString &icon, const QString &archive)
{
    this->setRowCount(rowCount()+1);

    QToolButton *toolbtn = new QToolButton(this);
    toolbtn->setIcon( QIcon(icon) );
    toolbtn->setIconSize(QSize(24, 24));
    toolbtn->setText( title );
    toolbtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    // new item within a button
    QTableWidgetItem *item = new QTableWidgetItem(title);
    QStringList dataList;
    dataList << title << icon << archive;
    item->setData( Qt::UserRole, dataList );
    this->setItem(rowCount() - 1, 0, item);

    this->setCellWidget( rowCount()-1, 0, toolbtn );

    connect( toolbtn, SIGNAL(clicked()), this, SLOT(buttonClicked()) );

    return toolbtn;
}

QToolButton* TableWidget::addButton(const QString &title)
{
    this->setRowCount(rowCount()+1);
    QToolButton *toolbtn = new QToolButton(this);
    toolbtn->setText( title );
    this->setCellWidget( rowCount()-1, 0, toolbtn );

    connect( toolbtn, SIGNAL(clicked()), this, SLOT(buttonClicked()) );

    return toolbtn;
}

QList<QToolButton *> TableWidget::itemButtons()
{
    QList<QToolButton *> btnList;
    this->rowCount();
    for(int i = 0; i < rowCount(); ++i)
    {
        QWidget *widget = this->cellWidget(i, 0);
        QToolButton* toolbtn = qobject_cast<QToolButton*>(widget);
        if( toolbtn )
            btnList.append(toolbtn);
    }

    return btnList;
}

QToolButton *TableWidget::itemButton(int row)
{
    QWidget *widget = this->cellWidget(row, 0);
    QToolButton* toolbtn = qobject_cast<QToolButton*>(widget);
    if( !toolbtn )
        return toolbtn=NULL;

    return toolbtn;
}

void TableWidget::buttonClicked()
{
    QToolButton *btn = qobject_cast<QToolButton *>(sender());
    for(int row = 0; row < this->rowCount(); ++row)
    {
        QWidget *widget = this->cellWidget(row, 0);
        QToolButton *btn1 = qobject_cast<QToolButton*>(widget);
        if( btn == btn1)
        {
            qDebug() << item(row, 0)->data(Qt::UserRole).toStringList().at(0)
                    << item(row, 0)->data(Qt::UserRole).toStringList().at(1)
                    << item(row, 0)->data(Qt::UserRole).toStringList().at(2);
        }
    }

}



