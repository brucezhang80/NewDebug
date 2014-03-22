#include "MainWidget.h"
#include "ui_MainWidget.h"
#include "CalendarWidget.h"

// qt header
#include <QGridLayout>
#include <QDebug>


MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    QGridLayout *grid = new QGridLayout(this);
    CalendarWidget *cal = new CalendarWidget(this);
    grid->addWidget(cal, 0, 0, 1, 1);

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnBottomHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);

    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(0xff,0xff,0xff,0));
    setPalette(pal);

}

MainWidget::~MainWidget()
{
    delete ui;
}

