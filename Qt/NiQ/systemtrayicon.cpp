#include "systemtrayicon.h"
#include <QApplication>
#include <QDebug>

SystemTrayIcon::SystemTrayIcon(QIcon icon, QWidget *parent) :
    QWidget(parent)
{
    m_thisParent = parent;
    createActions();
    createTrayIcon();
    m_trayIcon->setIcon(icon);
    connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            m_thisParent, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    m_trayIcon->show();
}

void SystemTrayIcon::setIcon(const QIcon icon)
{
    m_trayIcon->setIcon(icon);
}

void SystemTrayIcon::setToolTips(const QString &tooltip)
{
    m_trayIcon->setToolTip(tooltip);
}

void SystemTrayIcon::showTrayIconMessage(const QString &title, const QString &message, QSystemTrayIcon::MessageIcon icon, int millisecondsTimeoutHint)
{
    m_trayIcon->showMessage(title, message, icon, millisecondsTimeoutHint);
}

QRect SystemTrayIcon::geometry() const
{
    return m_trayIcon->geometry();
}

void SystemTrayIcon::createActions()
{
    m_hideAction = new QAction(tr("隐藏面板"), this);
    connect(m_hideAction, SIGNAL(triggered()), m_thisParent, SLOT(hide()));

    m_showAction = new QAction(tr("显示面板"), this);
    connect(m_showAction, SIGNAL(triggered()), this, SLOT(show()));

    m_restoreAction = new QAction(tr("还原"), this);
    connect(m_restoreAction, SIGNAL(triggered()), m_thisParent, SLOT(showNormal()));

    m_maximizeAction = new QAction(tr("最大化"), this);
    connect(m_maximizeAction, SIGNAL(triggered()), m_thisParent, SLOT(showMaximized()));

    m_quitAction = new QAction(tr("退出"), this);
    connect(m_quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void SystemTrayIcon::createTrayIcon()
{
    m_trayIconMenu = new QMenu(m_thisParent);
    m_trayIconMenu->addAction(m_hideAction);
    m_trayIconMenu->addAction(m_showAction);
    m_trayIconMenu->addAction(m_maximizeAction);
    m_trayIconMenu->addAction(m_restoreAction);
    m_trayIconMenu->addSeparator();
    m_trayIconMenu->addAction(m_quitAction);

    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setContextMenu(m_trayIconMenu);
}


void SystemTrayIcon::show()
{
    if(m_thisParent->isMinimized())
        m_thisParent->showNormal();
    else
        m_thisParent->show();
    this->raise();
    this->activateWindow();
}
