#pragma once
#ifndef PLUGIN_H
#define PLUGIN_H

class PluginManagerWidget;
class DockableWindow;

namespace Plugin
{
	extern PluginManagerWidget		*g_pluginMgrWidget;
	extern DockableWindow			*g_dockableWindow;

	void PluginStart();
	void PluginEnd();
};

#endif

