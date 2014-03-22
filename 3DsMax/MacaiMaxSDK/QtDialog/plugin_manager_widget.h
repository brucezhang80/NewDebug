#ifndef PLUGIN_MANAGER_WIDGET_H
#define PLUGIN_MANAGER_WIDGET_H

#include <QWidget>
#include <QAction>
#include "ui_plugin_manager_widget.h"

class PluginManagerWidget : public QWidget
{
	Q_OBJECT

public:
	PluginManagerWidget(QWidget *parent = 0);
	~PluginManagerWidget();

private slots:
    void dockToLeft();
    void dockToRight();
    void undock();
    void dock(bool isDock);

private:
    void setDcokBtnPopMenu();

	Ui::PluginManagerWidget ui;
    QAction     *m_dockLeftAct;
    QAction     *m_dockRightAct;
    QAction     *m_undockAct;

};

#endif // PLUGIN_MANAGER_WIDGET_H
