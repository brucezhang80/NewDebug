#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>

class HttpDownload;
//class ParseHtml;

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
    void updateProgressbar(qint64 completedBytes, qint64 totalBytes);
    void startDownload();
    void appendLog(const QString &log);
    void appendLogs(const QStringList &logs);
    void updateHtmlCount(int count);
    void updateImgCount(int count);
    void startNext();

    void updateUi();

private:
    Ui::MainWindow *ui;
//    ParseHtml*          m_parseHtml;
    HttpDownload*       m_httpDownload;
    QTime               m_downloadTime;
};

#endif // MAINWINDOW_H
