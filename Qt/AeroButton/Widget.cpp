/**************************************************************************
** Qt Creator license header template
**   Special keywords: Administrator 2013/12/20 2013
**   Environment variables:
**   To protect a percent sign, use '%'.
**************************************************************************/

#include "Widget.h"
#include "ui_Widget.h"


#include <QSqlDatabase>
#include <QSqlError>

#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setAutoFillBackground(true);
    QPalette pal;
    QPixmap pixmap(":/images/bk.jpg");
    pal.setBrush(QPalette::Window,QBrush(pixmap));
    this->setPalette(pal);

    m_aeroBtn = new AeroButton("Aero Button", this);
//    m_aeroBtn->setColor(QColor(Qt::darkRed));
//    m_aeroBtn->setShadow(QColor(Qt::black));
    m_aeroBtn->setHighlight(QColor(Qt::red));
    m_aeroBtn->setRoundness(5);
    m_aeroBtn->setOpacity(0.1);
    m_aeroBtn->setGeometry(100, 100, 160, 60);

    connect( m_aeroBtn, SIGNAL(clicked()), this, SLOT(connectMySql()) );
}

Widget::~Widget()
{
    delete ui;
}

void Widget::connectMySql()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("184.154.193.178");
    db.setDatabaseName("newdebug");
    db.setUserName("newdebug");
    db.setPassword("young1987");
    if( !db.open() )
    {
        qDebug() << "Can not open database. error: " << db.lastError().text();
        return;
    }

    qDebug() << "open database successful.";
}
