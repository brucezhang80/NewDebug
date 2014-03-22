#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "treeview_widget.h"

class DocModel;
class QMenu;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    bool openFile();

private:
    Ui::MainWindow *ui;
    QMenu                  *m_fileMenu;
    TreeViewWidget         *m_treeview;
    DocModel               *m_model;
    QString                 m_xmlPath;
};

#endif // MAINWINDOW_H
