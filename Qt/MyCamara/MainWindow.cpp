#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QCloseEvent>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QTime>
#include <QDate>

#define QT_OS_WIN
#include <qt_windows.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_index = 0;
    m_camara = NULL;
    m_image = new QImage();
    m_timer = new QTimer(this);

    // 时间到，读取当前摄像头信息
    connect(m_timer, SIGNAL(timeout()), this, SLOT(readFrame()));
    connect(ui->openCamBtn, SIGNAL(clicked()), this, SLOT(openCamara()));
    connect(ui->closeCamBtn, SIGNAL(clicked()), this, SLOT(closeCamara()));
    connect(ui->takePhotoBtn, SIGNAL(clicked()), this, SLOT(takePhoto()));

    connect(ui->photoList, SIGNAL(itemDoubleClicked ( QListWidgetItem*)),
            this, SLOT(openPhoto(QListWidgetItem*)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * 打开摄像头
 * @brief MainWindow::openCamara
 */
void MainWindow::openCamara()
{
    if( m_camara ) return;

    m_camara = cvCreateCameraCapture(0);
    m_timer->start(10);
    updateUI();
}

/**
 * 关闭摄像头
 * @brief MainWindow::closeCamara
 */
void MainWindow::closeCamara()
{
    m_timer->stop();
    cvReleaseCapture(&m_camara);
    ui->camViewer->clear();
    updateUI();
}

/**
 * 读取图像帧信息
 * @brief MainWindow::readFrame
 */
void MainWindow::readFrame()
{
    if( !m_camara ) return;

    // 缩放
//    CvSize size;
//    size.width = ui->camViewer->size().width();
//    size.height = ui->camViewer->size().height();
//    IplImage* srcFrame = cvQueryFrame(m_camara);
//    m_frame = cvCreateImage(size, srcFrame->depth, srcFrame->nChannels);
//    cvResize(srcFrame, m_frame, CV_INTER_LINEAR);

    m_frame = cvQueryFrame(m_camara);
    cvCvtColor(m_frame, m_frame, CV_BGR2RGB);
    QImage image((const uchar*)m_frame->imageData, m_frame->width,
                 m_frame->height, QImage::Format_RGB888);

    ui->camViewer->setPixmap(QPixmap::fromImage(image));
}

/**
 * 拍照生成图片保存到磁盘
 * @brief MainWindow::takePicture
 */
void MainWindow::takePhoto()
{
    if( !m_camara ) return;
    // 从摄像头中抓取并返回每一帧
    m_frame = cvQueryFrame(m_camara);
    cvCvtColor(m_frame, m_frame, CV_BGR2RGB);
    // 将抓取到的帧，转换为QImage格式
    //QImage::Format_RGB888不同的摄像头用不同的格式。
    QImage image = QImage((const uchar*)m_frame->imageData, m_frame->width,
                 m_frame->height, QImage::Format_RGB888);//.rgbSwapped();

    QString d = QDate::currentDate().toString("yyyy-MM-dd ");
    QString t =  QTime::currentTime().toString("hhmmss");
    QString phoName = QString("photo%1.jpg").arg(d+t);

    if ( savePhoto(image, phoName) )
        addPhotoToList(QPixmap::fromImage(image), phoName);
    else
        QMessageBox::information(this, tr("Taking Photo"),
                                 tr("Take photo failed, can not save photo to disk."),
                                 0);
}

/**
 * 拍照时，图片添加到缩略图列表中
 * @brief MainWindow::addPhotoToList
 * @param photo
 */
void MainWindow::addPhotoToList(const QPixmap &photo, const QString &name)
{
    //设置QListWidget中的单元项的图片大小
    ui->photoList->setIconSize(QSize(800, 60));
    ui->photoList->setResizeMode(QListView::Adjust);
       //设置QListWidget的显示模式
    ui->photoList->setViewMode(QListView::IconMode);
        //设置QListWidget中的单元项不可被拖动
    ui->photoList->setMovement(QListView::Static);

    //设置QListWidget中的单元项的间距
    ui->photoList->setSpacing(10);
    QListWidgetItem *pItem = new QListWidgetItem(QIcon(photo.scaled(QSize(80,60))),
                                                 name);
    pItem->setSizeHint(QSize(100,100)); //设置单元项的宽度和高度
    ui->photoList->addItem(pItem);       //添加QListWidgetItem项
}

bool MainWindow::savePhoto(const QImage &image, const QString &basename)
{
    QString savePath = QDir::currentPath() + "/photos/";
    QDir dir(savePath);
    if( !dir.exists(savePath) )
    {
        dir.mkdir(savePath);
    }

    bool b = image.save(savePath + basename, "PNG");
    if( ! b )
    {
        return false;
    }
    return true;
}

bool MainWindow::openPhoto(QListWidgetItem *item)
{
    QString savePath = QDir::currentPath() + "/photos/";
    QDir dir(savePath);
    if( !dir.exists(savePath) )
    {
        return false;
    }

    ShellExecuteW(NULL, QString("open").toStdWString().c_str(),
                  QString(savePath + item->text()).toStdWString().c_str(),
                  NULL, NULL, SW_SHOW);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if( m_camara )
        closeCamara();

    event->accept();
}

//void MainWindow::resizeEvent(QResizeEvent *event)
//{
//    ui->camViewer->resize(event->size());
//    event->accept();
//}

void MainWindow::updateUI()
{
    bool bState = true;
    if( !m_camara )
        bState = !bState;

    ui->closeCamBtn->setEnabled(bState);
    ui->takePhotoBtn->setEnabled(bState);
    ui->openCamBtn->setEnabled(!bState);
}







