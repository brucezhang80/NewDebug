#include "main_menu.h"
#include <imenuman.h> 
#include <imenus.h> 

ClassDesc2* GetMainMenuDesc() 
{ 
	static MainMenuClassDesc mainMenuDesc;
	return &mainMenuDesc; 
}

MainMenu::MainMenu(void)
{
	m_iMenuItem = NULL;
}

MainMenu::~MainMenu(void)
{
	if (m_iMenuItem)
	{
		delete m_iMenuItem;
		m_iMenuItem = NULL;
	}
}

DWORD MainMenu::Start()
{
	//QMfcApp::enterModalLoop();
	RegisterNotification(PostCreateUICallback, this, NOTIFY_SYSTEM_STARTUP);
	RegisterNotification(PostCreateUICallback, this, NOTIFY_SYSTEM_SHUTDOWN);
	return GUPRESULT_NOKEEP;
}

void MainMenu::Stop()
{
	UnRegisterNotification(PostCreateUICallback, this, NOTIFY_SYSTEM_STARTUP);
	UnRegisterNotification(PostCreateUICallback, this, NOTIFY_SYSTEM_SHUTDOWN);
}

DWORD_PTR MainMenu::Control( DWORD parameter )
{

	return 0;
}

IOResult MainMenu::Save( ISave *isave )
{

	return IO_OK;
}

IOResult MainMenu::Load( ILoad *iload )
{

	return IO_OK;
}

bool MainMenu::initMainMenu()
{
	Interface *core = GetCOREInterface();
	IMenuManager* menuMan = core->GetMenuManager();
	IMenu* mainMenuBar = menuMan->GetMainMenuBar();

	bool exist = false;
	for ( int index = 0; index < mainMenuBar->NumItems(); ++index )
	{
		if ( mainMenuBar->GetItem( index )->GetTitle() == CStr("iTool") )
		{
			exist = true;
			break;
		}
	}
	if ( !exist )
	{
		DynamicMenu dynamicMenu( &m_dmmCallback );
		dynamicMenu.BeginSubMenu( MENU_TITLE_MAIN );
		dynamicMenu.AddItem( 0, IDC_YMENU_PLUGIN, MENU_TITLE_PLUGIN );
		dynamicMenu.AddItem( 0, IDC_YMENU_CREATER, MENU_TITLE_CREATER );
		dynamicMenu.AddItem( m_dmmCallback.kSeparator, IDC_YMENU_SEPARATOR, "" );
		dynamicMenu.AddItem( 0, IDC_YMENU_MODIFIER, MENU_TITLE_MODIFIER);
		dynamicMenu.AddItem( 0, IDC_YMENU_EXPORTER, MENU_TITLE_EXPORTER );
		dynamicMenu.AddItem( 0, IDC_YMENU_IMPORTER, MENU_TITLE_IMPORTER );
		dynamicMenu.AddItem( m_dmmCallback.kSeparator, IDC_YMENU_SEPARATOR, "" );
		dynamicMenu.AddItem( 0, IDC_YMENU_TOOLBAR, MENU_TITLE_TOOLBAR );
		dynamicMenu.AddItem( 0, IDC_YMENU_SETTING, MENU_TITLE_SETTING );
		dynamicMenu.AddItem( m_dmmCallback.kSeparator, IDC_YMENU_SEPARATOR, "" );

		dynamicMenu.BeginSubMenu( MENU_TITLE_HELPER );
		dynamicMenu.AddItem( 0, IDC_YMENU_HELP, MENU_TITLE_HELP );
		dynamicMenu.AddItem( 0, IDC_YMENU_ABOUT, MENU_TITLE_ABOUT );
		dynamicMenu.EndSubMenu();

		dynamicMenu.EndSubMenu();

		m_iMenuItem = dynamicMenu.GetMenu()->GetItem( 0 );
		m_iMenuItem->EnableActions(TRUE);
		mainMenuBar->AddItem( m_iMenuItem, mainMenuBar->NumItems()-1);
		menuMan->UpdateMenuBar();

		exist = true;
		return true;
	}

	return false;
}

bool MainMenu::uninitManiMenu()
{
	Interface *core = GetCOREInterface();
	IMenuManager* menuMan = core->GetMenuManager();
	IMenu* mainMenuBar = menuMan->GetMainMenuBar();

	for ( int index = 0; index < mainMenuBar->NumItems(); ++index )
	{
		CStr title = mainMenuBar->GetItem( index )->GetTitle();
		if ( title == CStr(MENU_TITLE_MAIN) )
		{
			mainMenuBar->RemoveItem(index);
			menuMan->UpdateMenuBar();
			return true;	
		}
	}
	return false;
}

void MainMenu::PostCreateUICallback( void* param, NotifyInfo* info )
{
	MainMenu * pThis = (MainMenu*) param;
	DbgAssert(pThis != NULL);
    //Interface* pMaxInterface = GetCOREInterface();
    //HWND hMax = pMaxInterface->GetMAXHWnd();

	switch ( info->intcode )
	{
	case NOTIFY_SYSTEM_STARTUP:
		{
			pThis->initMainMenu();
		};
		break;
	case NOTIFY_SYSTEM_SHUTDOWN:
		{
			pThis->uninitManiMenu();
		};
		break;
	default:
		return;
	}
}
