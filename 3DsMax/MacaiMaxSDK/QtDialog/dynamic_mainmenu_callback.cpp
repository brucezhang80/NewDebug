#include "dynamic_mainmenu_callback.h"
#include "constants.h"
#include "Plugin.h"
#include "DockableWindow.h"

#include <maxscrpt/maxscrpt.h>
#include <time.h>
#include <imenuman.h> 
#include <imenus.h>

DynamicMainMenuCallback::DynamicMainMenuCallback(void)
{
}

DynamicMainMenuCallback::~DynamicMainMenuCallback(void)
{
}

void DynamicMainMenuCallback::MenuItemSelected( int itemId /*= 0 */ )
{
	Interface *core = GetCOREInterface();
	HWND hMax = core->GetMAXHWnd();

	switch (itemId)
	{
	case IDC_YMENU_PLUGIN:		//插件集
		{
			if( !Plugin::g_dockableWindow )
				Plugin::PluginStart();
			else
			{
				if (Plugin::g_dockableWindow->m_frame->IsHidden())
				{
					Plugin::g_dockableWindow->m_frame->Hide(false);
				}
			}
		};
		break;
	case IDC_YMENU_CREATER:	//模型创造器
		{

		};
		break;
	case IDC_YMENU_MODIFIER://场景资源修改器
		{
			
		};
		break;
	case IDC_YMENU_EXPORTER:	//场景导出器
		{
			MessageBox( hMax, "导出工具", "导出", 0 );
		};
		break;

	case IDC_YMENU_IMPORTER:	//导入器
		{
			MessageBox( hMax, "导入工具", "导入", 0 );
		};
		break;
	case IDC_YMENU_TOOLBAR:	//工具栏
		{
			MessageBox( hMax, "显示工具栏", "工具栏", 0 );
		};
		break;
	case IDC_YMENU_SETTING:	//设置
		{
			MessageBox( hMax, "显示设置对话框", "设置", 0 );
		};
		break;
	case IDC_YMENU_HELP:	//帮助主题
		{
			MessageBox( hMax, "帮助主题", "帮助", 0 );
		};
		break;
	case IDC_YMENU_ABOUT:	//关于我们
		{
			MessageBox( hMax, "关于我们", "关于", 0 );
		};
		break;

	default:break;
	}
}
