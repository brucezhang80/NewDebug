#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QTimer>
#include <QListWidgetItem>

#include <highgui.h>  //包含opencv库头文件
#include <cv.h>

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
    void openCamara();
    void closeCamara();
    void readFrame();
    void takePhoto();
    void addPhotoToList(const QPixmap &photo, const QString &name);
    bool savePhoto(const QImage &image, const QString &basename);
    bool openPhoto(QListWidgetItem *item);

protected:
    void closeEvent(QCloseEvent *);
//    void resizeEvent(QResizeEvent *);

private:
    void updateUI();

private:
    Ui::MainWindow *ui;
    QTimer*     m_timer;
    QImage*     m_image;
    CvCapture*  m_camara;   // 视频获取结构， 用来作为视频获取函数的一个参数
    IplImage*   m_frame;    // 申请IplImage类型指针，就是申请内存空间来存放每一帧图像
    int         m_index;
};

#endif // MAINWINDOW_H
