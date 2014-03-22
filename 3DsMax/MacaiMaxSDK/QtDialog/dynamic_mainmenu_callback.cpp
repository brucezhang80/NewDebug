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
	case IDC_YMENU_PLUGIN:		//�����
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
	case IDC_YMENU_CREATER:	//ģ�ʹ�����
		{

		};
		break;
	case IDC_YMENU_MODIFIER://������Դ�޸���
		{
			
		};
		break;
	case IDC_YMENU_EXPORTER:	//����������
		{
			MessageBox( hMax, "��������", "����", 0 );
		};
		break;

	case IDC_YMENU_IMPORTER:	//������
		{
			MessageBox( hMax, "���빤��", "����", 0 );
		};
		break;
	case IDC_YMENU_TOOLBAR:	//������
		{
			MessageBox( hMax, "��ʾ������", "������", 0 );
		};
		break;
	case IDC_YMENU_SETTING:	//����
		{
			MessageBox( hMax, "��ʾ���öԻ���", "����", 0 );
		};
		break;
	case IDC_YMENU_HELP:	//��������
		{
			MessageBox( hMax, "��������", "����", 0 );
		};
		break;
	case IDC_YMENU_ABOUT:	//��������
		{
			MessageBox( hMax, "��������", "����", 0 );
		};
		break;

	default:break;
	}
}
