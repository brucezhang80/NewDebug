#include "Plugin.h"
#include "DockableWindow.h"
#include "plugin_manager_widget.h"

#include <QtCore>
#include <QtGui>
#include <qmfcapp.h>
#include <qwinwidget.h>
#include <QHBoxLayout>
#include <QPushButton>
#include <QGridLayout>
#include <QWidget>

using namespace Plugin;

PluginManagerWidget		*Plugin::g_pluginMgrWidget = 0;
DockableWindow			*Plugin::g_dockableWindow = 0;


void Plugin::PluginStart()
{
    g_dockableWindow = DockableWindow::Create(_T("QT In 3Dsmax Demo"));
	HWND hwnd = g_dockableWindow->GetHWND();
	QWinWidget *qWin=new QWinWidget(hwnd);
	qWin->resize(225,480);
	qWin->setContentsMargins(0, 0, 0, 0);
	QGridLayout *gridlayout = new QGridLayout;
	gridlayout->setContentsMargins(0, 0, 0, 0);
	gridlayout->setSpacing(0);
	g_pluginMgrWidget = new PluginManagerWidget( qWin );
	gridlayout->addWidget(g_pluginMgrWidget);
	qWin->setLayout(gridlayout);

	g_dockableWindow->SetWidget(qWin);
	//g_dockableWindow->ResizeFrameToContent();
	//g_dockableWindow->Dock(DockableWindow::Left | DockableWindow::Right);
	//g_dockableWindow->ResizeContentToFrame();
}

void Plugin::PluginEnd()
{

}
