#ifndef QTDIALOG_H
#define QTDIALOG_H

#include "qtdialog_global.h"
#include <Qt>
#include <QtGui>
#include <qmfcapp.h>
#include <qwinwidget.h>
#include <windows.h>

class QTDIALOG_EXPORT QtDialog : public QWinWidget
{
public:
	QtDialog(HWND hParentWnd, QObject *parent = 0, Qt::WFlags f = 0 );
	~QtDialog();

private:

};

#endif // QTDIALOG_H
