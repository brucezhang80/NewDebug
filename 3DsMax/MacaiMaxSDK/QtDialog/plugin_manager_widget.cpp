#include "plugin_manager_widget.h"
#include "DockableWindow.h"
#include "Plugin.h"
#include "constants.h"
#include <QMenu>


PluginManagerWidget::PluginManagerWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
    setDcokBtnPopMenu();

    connect(ui.dockToBtn, SIGNAL(toggled(bool)), this, SLOT(dock(bool)));
}

PluginManagerWidget::~PluginManagerWidget()
{

}

void PluginManagerWidget::dockToLeft()
{
    Plugin::g_dockableWindow->Dock(DockableWindow::Left);
    ui.dockToBtn->setText(tr("Undock"));
    ui.dockToBtn->setChecked(true);
}

void PluginManagerWidget::dockToRight()
{
    Plugin::g_dockableWindow->Dock(DockableWindow::Right);
    ui.dockToBtn->setText(tr("Undock"));
    ui.dockToBtn->setChecked(true);
}

void PluginManagerWidget::undock()
{
    Plugin::g_dockableWindow->Float();
    ui.dockToBtn->setText(tr("Dock"));
    ui.dockToBtn->setChecked(false);
}

void PluginManagerWidget::dock(bool isDock)
{
    if( isDock )
    {
        Plugin::g_dockableWindow->Dock(DockableWindow::Left);
        ui.dockToBtn->setText(tr("Undock"));
    }
    else
    {
        Plugin::g_dockableWindow->Float();
        ui.dockToBtn->setText(tr("Dock"));
    }
}

void PluginManagerWidget::setDcokBtnPopMenu()
{
    QMenu *dockMenu = new QMenu(this);
    m_undockAct = new QAction(this);
    dockMenu->addAction(m_undockAct);
    m_undockAct->setText(tr("&Undock"));
    connect(m_undockAct, SIGNAL(triggered()), this, SLOT(undock()));

    m_dockLeftAct = new QAction(this);
    dockMenu->addAction(m_dockLeftAct);
    m_dockLeftAct->setText(tr("Dock &Left"));
    connect(m_dockLeftAct, SIGNAL(triggered()), this, SLOT(dockToLeft()));

    m_dockRightAct = new QAction(this);
    dockMenu->addAction(m_dockRightAct);
    m_dockRightAct->setText(tr("Dock &Right"));
    connect(m_dockRightAct, SIGNAL(triggered()), this, SLOT(dockToRight()));

    ui.dockToBtn->setMenu(dockMenu);
}
