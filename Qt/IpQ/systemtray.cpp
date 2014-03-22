#include "systemtray.h"
#include "aboutdialog.h"
#include "constants.h"
#include "macai.h"
#include "system.h"
#include "message_box.h"
#include "transfer_file_window.h"
#include "system_settings.h"

#include <QApplication>
#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>
#include <QMouseEvent>
#include <QHoverEvent>
#include <QIcon>
#include <QMessageBox>

#include <QDebug>

static const QString g_tooltip = QObject::tr("Name:") + System::hostName()\
        + QString("\nIP:") + System::hostAddress()\
        + QObject::tr("\nIPQ Version 0.1\nCopyRight (c) 2013 Power By Macai");
static const QString g_iconFile = QString(":/images/IPQ.png"); //这里的图标设置后期需要从主题文件中设置图标

SystemTray::SystemTray(QObject *parent) :
    QObject(parent)
{
    createActions();
    retranslateUi();
    createTrayIcon();

    connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));

}

SystemTray::~SystemTray()
{
    delete m_trayIcon;
    m_trayIcon = 0;
}

void SystemTray::retranslateUi()
{
    m_transferAction->setText(tr("&Open Transfer"));
    m_hideAction->setText(tr("&Hide panel"));
    m_showAction->setText(tr("&Show panel"));
    m_configAction->setText(tr("&Setting"));
    m_aboutAction->setText(tr("&About"));
    m_quitAction->setText(tr("&Exit"));
}

void SystemTray::create()
{
    m_trayIcon->show();
    show();
}

QRect SystemTray::geometry()
{
    return m_trayIcon->geometry();
}

void SystemTray::showTooltip(bool bShow)
{
    if(bShow)
        m_trayIcon->setToolTip(g_tooltip);
    else
        m_trayIcon->setToolTip("");
}

void SystemTray::show()
{
    Macai::mainWindow->show();
    Macai::mainWindow->raise();
    Macai::mainWindow->activateWindow();
}

void SystemTray::hide()
{
    Macai::mainWindow->hide();
}

void SystemTray::about()
{
    Macai::aboutDialog->show();
}

void SystemTray::quit()
{
    qApp->quit();
}

void SystemTray::transfer()
{
    Macai::transferFileWindow->show();
}

void SystemTray::config()
{
    Macai::systemSettings->show();
}

void SystemTray::createActions()
{
    m_transferAction = new QAction( this);
    connect(m_transferAction, SIGNAL(triggered()), this, SLOT(transfer()));

    m_hideAction = new QAction(this);
    connect(m_hideAction, SIGNAL(triggered()), this, SLOT(hide()));

    m_showAction = new QAction(this);
    connect(m_showAction, SIGNAL(triggered()), this, SLOT(show()));

    m_configAction = new QAction(this);
    connect(m_configAction, SIGNAL(triggered()), this, SLOT(config()));

    m_aboutAction = new QAction(this);
    connect(m_aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    m_quitAction = new QAction(this);
    connect(m_quitAction, SIGNAL(triggered()), this, SLOT(quit()));
}

void SystemTray::createTrayIcon()
{
    m_trayIconMenu = new QMenu(Macai::mainWindow->window());
    m_trayIconMenu->addAction(m_transferAction);
    m_trayIconMenu->addSeparator();
    m_trayIconMenu->addAction(m_hideAction);
    m_trayIconMenu->addAction(m_showAction);
    m_trayIconMenu->addSeparator();
    m_trayIconMenu->addAction(m_configAction);
    m_trayIconMenu->addAction(m_aboutAction);
    m_trayIconMenu->addSeparator();
    m_trayIconMenu->addAction(m_quitAction);

    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setContextMenu(m_trayIconMenu);
    m_trayIcon->setToolTip(g_tooltip);
    m_trayIcon->setIcon(QIcon(g_iconFile));//这里的图标设置后期需要从主题文件中设置图标
}

void SystemTray::createMainWindow()
{
    Macai::mainWindow->show();
}

void SystemTray::destroyMainWindow()
{
    Macai::mainWindow->close();
}

void SystemTray::visibleMainWindow()
{
   // mainWindow->setWindowFlags(mainWindow->windowFlags() | Qt::WindowStaysOnTopHint);
    Macai::mainWindow->showNormal();
    Macai::mainWindow->raise();
    Macai::mainWindow->activateWindow();
    //mainWindow->setWindowFlags(mainWindow->windowFlags() & ~Qt::WindowStaysOnTopHint);
}

void SystemTray::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Trigger:
//        Macai::messageBox->show();
        break;
    case QSystemTrayIcon::DoubleClick:
        visibleMainWindow();
        break;
    case QSystemTrayIcon::MiddleClick:
        break;
    default:
        return;
    }
}

