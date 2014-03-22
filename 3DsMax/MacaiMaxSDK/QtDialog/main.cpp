/****************************************************************************
****************************************************************************/
#include "main_menu.h"
#include <qmfcapp.h>
#include <qwinwidget.h>
#include <QTextCodec>

extern ClassDesc2* GetMainMenuDesc();

HINSTANCE hInstance;
static bool ownApplication = FALSE;

BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD dwReason, LPVOID /*lpvReserved*/ )
{
//     if ( dwReason == DLL_PROCESS_ATTACH )
// 	{
// 		hInstance = hinstDLL;
// 		ownApplication = QMfcApp::pluginInstance( hInstance );
// 		DisableThreadLibraryCalls(hInstance);
// 	}
//     if ( dwReason == DLL_PROCESS_DETACH && ownApplication )
// 		delete qApp;

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		hInstance = hinstDLL;
		QMfcApp::pluginInstance(hInstance);
	}

	QTextCodec *codec = QTextCodec::codecForName("system");
	QTextCodec::setCodecForLocale(codec);
	QTextCodec::setCodecForCStrings(codec);
	QTextCodec::setCodecForTr(codec);

    return TRUE;
}

extern "C" __declspec(dllexport) const TCHAR* LibDescription()
{
	return IDS_LIBDESCRIPTION;
}

extern "C" __declspec(dllexport) int LibNumberClasses()
{
	return 1;
}

extern "C" __declspec(dllexport) ClassDesc* LibClassDesc(int i)
{
	switch(i)
	{
		case 0: return GetMainMenuDesc();
		default: return 0;
	}
}

extern "C" __declspec(dllexport) ULONG LibVersion()
{
	return VERSION_3DSMAX;
}

extern "C" __declspec(dllexport)int LibInitialize(void)
{
	return TRUE; // TODO: Perform initialization here.
}

extern "C" __declspec(dllexport) int LibShutdown(void)
{
	return TRUE;// TODO: Perform un-initialization here.	
}

TCHAR *GetString(int id)
{
	static TCHAR buf[256];

	if (hInstance)
		return LoadString(hInstance, id, buf, sizeof(buf)) ? buf : NULL;
	return NULL;
}
