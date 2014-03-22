#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Server;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
public slots:
    void onStartServer(bool on);

private:
    Ui::MainWindow *ui;

    Server*     m_server;
};

#endif // MAINWINDOW_H
