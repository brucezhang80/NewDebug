#include <QtCore>
#include <QtGui>
#include <QApplication>

#include "quickviewer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QuickViewer w;
    w.show();

    return a.exec();
}
