#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QtGui>
#include <QMainWindow>
#include <QMap>
#include <QFileInfoList>
#include <QChildEvent>

//class UserTreeWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void retranslateUi();

private slots:
    void showSetting();
    void showTransfer();

private:
    void initWindow();
    void setupUi();
    void freeUi();
    QWidget     *centralwidget;
    QGridLayout *gridLayout;
    QGridLayout *gridLayout_2;
    QLabel      *lb_avatar;
    QLabel      *lb_name;
    QLabel      *lb_officeTitle;
    QLabel      *lb_ip;
    QLabel      *lb_tmp;
    QSpacerItem *horizontalSpacer;
    QLineEdit   *ledt_search;
    QToolButton *settingBtn;
    QToolButton *sendFileBtn;

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *e);
};

#endif // MAIN_WINDOW_H
