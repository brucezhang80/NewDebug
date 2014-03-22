#pragma once
//#include "actiontable.h"
#include <Max.h>
#include <stdmat.h>
#include <imtl.h>
#include "constants.h"

class DockableWindow;
class MainWidget;

class DynamicMainMenuCallback : public DynamicMenuCallback
{
public:
	DynamicMainMenuCallback(void);
	~DynamicMainMenuCallback(void);
	void virtual MenuItemSelected( int itemId = 0 );

	enum DynamicMenuFlags { kDisabled = 1 << 0, kChecked = 1 << 1, kSeparator = 1 << 2 }; 
};
