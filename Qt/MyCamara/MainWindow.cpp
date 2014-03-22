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

    // ʱ�䵽����ȡ��ǰ����ͷ��Ϣ
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
 * ������ͷ
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
 * �ر�����ͷ
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
 * ��ȡͼ��֡��Ϣ
 * @brief MainWindow::readFrame
 */
void MainWindow::readFrame()
{
    if( !m_camara ) return;

    // ����
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
 * ��������ͼƬ���浽����
 * @brief MainWindow::takePicture
 */
void MainWindow::takePhoto()
{
    if( !m_camara ) return;
    // ������ͷ��ץȡ������ÿһ֡
    m_frame = cvQueryFrame(m_camara);
    cvCvtColor(m_frame, m_frame, CV_BGR2RGB);
    // ��ץȡ����֡��ת��ΪQImage��ʽ
    //QImage::Format_RGB888��ͬ������ͷ�ò�ͬ�ĸ�ʽ��
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
 * ����ʱ��ͼƬ��ӵ�����ͼ�б���
 * @brief MainWindow::addPhotoToList
 * @param photo
 */
void MainWindow::addPhotoToList(const QPixmap &photo, const QString &name)
{
    //����QListWidget�еĵ�Ԫ���ͼƬ��С
    ui->photoList->setIconSize(QSize(800, 60));
    ui->photoList->setResizeMode(QListView::Adjust);
       //����QListWidget����ʾģʽ
    ui->photoList->setViewMode(QListView::IconMode);
        //����QListWidget�еĵ�Ԫ��ɱ��϶�
    ui->photoList->setMovement(QListView::Static);

    //����QListWidget�еĵ�Ԫ��ļ��
    ui->photoList->setSpacing(10);
    QListWidgetItem *pItem = new QListWidgetItem(QIcon(photo.scaled(QSize(80,60))),
                                                 name);
    pItem->setSizeHint(QSize(100,100)); //���õ�Ԫ��Ŀ�Ⱥ͸߶�
    ui->photoList->addItem(pItem);       //���QListWidgetItem��
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







