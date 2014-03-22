#pragma once

//3dsmaxsdk preinclude 
#define STRING2(x) #x
#define STRING(x) STRING2(x)
#define TODO(x) __FILE__ "(" STRING(__LINE__) "): TODO: "x

#include <Max.h>
#include <istdplug.h>
#include <iparamb2.h>
#include <iparamm2.h>
//SIMPLE TYPE
#include <actiontable.h>
#include <notify.h>
#include <guplib.h>

#include "constants.h"
#include "dynamic_mainmenu_callback.h"

extern TCHAR *GetString(int id);
extern HINSTANCE hInstance;

class MainMenu : public GUP
{
public:
	MainMenu(void);
	~MainMenu(void);

public:
	IMenuItem*		m_iMenuItem;
	static HWND		hParams;

	// GUP Methods
	DWORD		Start			( );
	void		Stop			( );
	DWORD_PTR	Control			( DWORD parameter );

	// Loading/Saving
	IOResult Save(ISave *isave);
	IOResult Load(ILoad *iload);	

public:
	DynamicMainMenuCallback m_dmmCallback;

public:
	bool initMainMenu();
	bool uninitManiMenu();

protected:
	static void	PostCreateUICallback(void* param, NotifyInfo* info);

};

static MainMenu *mainMenu;

class MainMenuClassDesc : public ClassDesc2 
{
public:
	virtual int IsPublic() 							{ return TRUE; }
	virtual void* Create(BOOL /*loading = FALSE*/) 	{ return new MainMenu(); }
	virtual const TCHAR *	ClassName() 			{ return IDS_CLASS_NAME; }
	virtual SClass_ID SuperClassID() 				{ return GUP_CLASS_ID; }
	virtual Class_ID ClassID() 						{ return MainMenu_CLASS_ID; }
	virtual const TCHAR* Category() 				{ return IDS_CATEGORY; }

	virtual const TCHAR* InternalName() 			{ return _T("MainMenu"); }	// returns fixed parsable name (scripter-visible name)
	virtual HINSTANCE HInstance() 					{ return hInstance; }					// returns owning module handle


};