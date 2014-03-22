#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QTimer>
#include <QListWidgetItem>

#include <highgui.h>  //����opencv��ͷ�ļ�
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
    CvCapture*  m_camara;   // ��Ƶ��ȡ�ṹ�� ������Ϊ��Ƶ��ȡ������һ������
    IplImage*   m_frame;    // ����IplImage����ָ�룬���������ڴ�ռ������ÿһ֡ͼ��
    int         m_index;
};

#endif // MAINWINDOW_H
