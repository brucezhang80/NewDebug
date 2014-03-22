#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QSignalMapper>

//class SideBar;
//class JobSenderView;
//class TableView;
//class ListView;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
    bool initMainWindow();
    void initStatusbar();

signals:

private slots:
    void showStatusbarTime();
    void updateSenderBtn(bool);

private:
    void setSidebarBtnMap();

public:
//    QWidget         *m_centralWidget;
//    QGridLayout     *m_gridLayout;
//    QSplitter       *m_splitter;
//    JobSenderView   *m_jobSenderView;
//    TableView       *m_tableView;
//    ListView        *m_listView;
private:
    Ui::MainWindow *ui;
    QLabel *timeStatus;

    QSignalMapper   *m_sidebarBtnMapping;
};

#endif // MAINWINDOW_H
