#include "MainWidget.h"
#include "ui_MainWidget.h"
#include "PixmapView.h"
#include "ThumbnailView.h"
#include "ThumbnailBar.h"

#include <QtGui>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QAbstractScrollArea>
#include <QImageReader>
#include <QFileInfo>
#include <QResizeEvent>
#include <QDir>

#include <QDebug>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    this->setMinimumSize(400, 280);
    m_pixmaphView = new PixmapView;
    m_thumbnailBar = new ThumbnailBar;
    m_editPath = new QLineEdit("I:\\Yvi\\images");
    QPushButton* btnGo = new QPushButton("Go");

    QGridLayout* grid = new QGridLayout(m_pixmaphView);
    grid->setMargin(0);
    grid->addWidget(m_editPath, 0, 0, 1, 3);
    grid->addWidget(btnGo, 0, 3, 1, 1);
    QSpacerItem* verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    grid->addItem(verticalSpacer, 1, 1, 1, 1);
    grid->addWidget(m_thumbnailBar, 2, 0, 1, 6);

    ui->gridLayout->addWidget(m_pixmaphView, 1, 0, 1, 1);
    this->setLayout(ui->gridLayout);

    connect(btnGo, SIGNAL(clicked()), this, SLOT(loadImages()));
    connect(m_thumbnailBar, SIGNAL(thumbnailChanged(QString)), this, SLOT(imageChanged(QString)));
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::imageChanged(const QString &filename)
{
    QFileInfo info(filename);
    if( !info.exists() )
    {
        qDebug() << filename << "dose not exists.";
        return;
    }
    QImageReader reader;
    reader.setFileName(filename);
    QPixmap pix = QPixmap::fromImageReader( &reader );
//    int w = pix.width();
//    int h = pix.height();

    m_pixmaphView->setImage(pix);
}

void MainWidget::loadImages()
{
    if( m_editPath->text().isEmpty()) return;

    QString path = m_editPath->text();
    QFileInfo info(path);
    if(info.isFile())
    {
        path = info.absoluteDir().absolutePath();
        imageChanged(path);
    }

    m_thumbnailBar->loadImages(path);
}

void MainWidget::resizeEvent(QResizeEvent *event)
{
    m_thumbnailBar->hide();
    QWidget::resizeEvent(event);
}
