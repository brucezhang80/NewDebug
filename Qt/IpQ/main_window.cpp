#include "macai.h"
#include "system.h"
#include "usertreewidget.h"
#include "main_window.h"
#include "system_settings.h"
#include "transfer_file_window.h"

#include <QApplication>
#include <QFile>
#include <QIcon>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
    initWindow();

    connect(settingBtn, SIGNAL(clicked()), this, SLOT(showSetting()) );
    connect(sendFileBtn, SIGNAL(clicked()), this, SLOT(showTransfer()) );

}

MainWindow::~MainWindow()
{
    freeUi();
}

void MainWindow::initWindow()
{
    this->resize(QSize(200, 600));
    this->setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    this->setWindowTitle(tr("IPQ"));
    this->setWindowIcon(QIcon(QString(":/images/IPQ.png")));

    lb_name->setText(System::hostName());
    lb_ip->setText(System::hostAddress());
}

void MainWindow::setupUi()
{
    centralwidget = new QWidget(this);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    gridLayout = new QGridLayout(centralwidget);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout_2 = new QGridLayout();
    gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
    lb_avatar = new QLabel(centralwidget);
    lb_avatar->setObjectName(QString::fromUtf8("lb_avatar"));
    lb_avatar->setMinimumSize(QSize(60, 0));
    lb_avatar->setMaximumSize(QSize(55, 60));
    lb_avatar->setPixmap(QPixmap(QString::fromUtf8(":/images/buddy-blue.png")));
    lb_avatar->setScaledContents(true);
    lb_avatar->setAlignment(Qt::AlignCenter);

    gridLayout_2->addWidget(lb_avatar, 0, 0, 3, 1);

    lb_name = new QLabel(centralwidget);
    lb_name->setObjectName(QString::fromUtf8("lb_name"));
    lb_name->setMinimumSize(QSize(100, 0));

    gridLayout_2->addWidget(lb_name, 0, 2, 1, 1);

    lb_officeTitle = new QLabel(centralwidget);
    lb_officeTitle->setObjectName(QString::fromUtf8("lb_officeTitle"));

    gridLayout_2->addWidget(lb_officeTitle, 1, 2, 1, 2);

    lb_ip = new QLabel(centralwidget);
    lb_ip->setObjectName(QString::fromUtf8("lb_ip"));

    gridLayout_2->addWidget(lb_ip, 2, 2, 1, 1);

    lb_tmp = new QLabel(centralwidget);
    lb_tmp->setObjectName(QString::fromUtf8("lb_tmp"));

    gridLayout_2->addWidget(lb_tmp, 2, 3, 1, 1);

    horizontalSpacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    gridLayout_2->addItem(horizontalSpacer, 0, 3, 1, 1);
    gridLayout->addLayout(gridLayout_2, 0, 0, 1, 1);

    ledt_search = new QLineEdit(centralwidget);
    ledt_search->setObjectName(QString::fromUtf8("ledt_search"));

    gridLayout->addWidget(ledt_search, 1, 0, 1, 1);

    Macai::userTreeWidget->setParent(centralwidget);
//    userTreeWidget = new UserTreeWidget(centralwidget);
    Macai::userTreeWidget->setObjectName(QString::fromUtf8("userTreeWidget"));
    Macai::userTreeWidget->setIconSize(QSize(24, 24));
    Macai::userTreeWidget->setAnimated(true);
    Macai::userTreeWidget->setHeaderHidden(true);

    gridLayout->addWidget(Macai::userTreeWidget, 2, 0, 1, 1);

    QHBoxLayout *hLayout = new QHBoxLayout(centralwidget);
    settingBtn = new QToolButton;
    settingBtn->setFixedSize(20,20);
    settingBtn->setIcon(QIcon(":/images/setting.png"));
    sendFileBtn = new QToolButton;
    sendFileBtn->setFixedSize(20,20);
    sendFileBtn->setIcon(QIcon(":/images/file-send.png"));

    QSpacerItem  *horizontalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    hLayout->addWidget(settingBtn);
    hLayout->addWidget(sendFileBtn);
    hLayout->addItem(horizontalSpacer_2);
    hLayout->setContentsMargins(0,0,0,0);
    gridLayout->addLayout(hLayout, 3,0,1,1);

    setCentralWidget(centralwidget);
    retranslateUi();
}

void MainWindow::retranslateUi()
{
    this->setWindowTitle(QApplication::translate("IPQ", "MainWindow", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
    lb_avatar->setToolTip(QApplication::translate("IPQ", "My avatar", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
    lb_avatar->setText(QString());
#ifndef QT_NO_TOOLTIP
    lb_name->setToolTip(QApplication::translate("IPQ", "My name", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
    lb_name->setText(QApplication::translate("IPQ", "Name", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
    lb_officeTitle->setToolTip(QApplication::translate("IPQ", "My group in", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
    lb_officeTitle->setText(QApplication::translate("IPQ", "Office Title", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
    lb_ip->setToolTip(QApplication::translate("IPQ", "My ip address", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
    lb_ip->setText(QApplication::translate("IPQ", "00.00.00.00:port", 0, QApplication::UnicodeUTF8));
    lb_tmp->setText(QString());
#ifndef QT_NO_TOOLTIP
    ledt_search->setToolTip(QApplication::translate("IPQ", "Search my friend", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
    ledt_search->setText(QApplication::translate("IPQ", "Search...", 0, QApplication::UnicodeUTF8));
}

void MainWindow::freeUi()
{
    centralwidget->deleteLater();
    gridLayout->deleteLater();
    gridLayout_2->deleteLater();
    lb_avatar->deleteLater();
    lb_name->deleteLater();
    lb_officeTitle->deleteLater();
    lb_ip->deleteLater();
    lb_tmp->deleteLater();
    //horizontalSpacer->deleteLater();
    ledt_search->deleteLater();
    Macai::userTreeWidget->deleteLater();
}

void MainWindow::showSetting()
{
    Macai::systemSettings->show();
}

void MainWindow::showTransfer()
{
    Macai::transferFileWindow->show();
}

// 最小化时不停留在任务栏
void MainWindow::changeEvent(QEvent *e)
{
    if((e->type() == QEvent::WindowStateChange) && this->isMinimized())
    {
        QTimer::singleShot(200, this, SLOT(hide()));
    }
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    QMessageBox queryMsgBox(this);
    queryMsgBox.setText( tr("Do you went to quit IPQ?") );
    queryMsgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::No);
    int ret = queryMsgBox.exec();

    switch (ret)
    {
    case QMessageBox::Ok:
        e->accept();
        qApp->quit();
        break;
    case QMessageBox::No:
        e->ignore();
        break;
    }
}
