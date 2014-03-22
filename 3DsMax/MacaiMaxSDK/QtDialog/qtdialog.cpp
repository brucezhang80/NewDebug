#include "qtdialog.h"


QtDialog::QtDialog( HWND hParentWnd, QObject *parent /*= 0*/, Qt::WFlags f /*= 0 */ )
	:QWinWidget( hParentWnd, parent, f)
{

}

QtDialog::~QtDialog()
{

}

