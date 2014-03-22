/**************************************************************************
** Qt Creator license header template
**   Special keywords: Administrator 2013/12/6 2013
**   Environment variables:
**   To protect a percent sign, use '%'.
**************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QByteArray>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:

private slots:
    void updateInfo(int type);
    void loginFinished(int type, const QNetworkRequest &request, const QByteArray &requestData);
    void updateBtn(bool);
    void startPanicBuying();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
