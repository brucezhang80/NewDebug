#pragma once
#ifndef PLUGIN_H
#define PLUGIN_H

#ifdef Q_WS_WIN
#define SCRIPTS_EXPORT __declspec(dllexport)
#else
#define SCRIPTS_EXPORT
#endif

#include <QString>

class ScriptsWidget;
class DockableWindow;

namespace Plugin
{
    extern ScriptsWidget        *g_scriptsWidget;
    extern DockableWindow		*g_dockableWindow;

    extern bool                  g_exsits;
	void PluginStart();
	void PluginEnd();

    QString pluginPath();
    QString dataPath();
}

#endif

