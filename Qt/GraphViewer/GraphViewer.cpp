#include "GraphViewer.h"
#include "ui_GraphViewer.h"
#include "PixmapView.h"
#include "ThumbnailView.h"
#include "ThumbnailBar.h"

#include "qwin/qmfcapp.h"
#include "qwin/qwinwidget.h"

#include <QGraphicsView>
#include <QHBoxLayout>
#include <QPushButton>
#include <QAbstractScrollArea>
#include <QImageReader>
#include <QFileInfo>
#include <QResizeEvent>
#include <QDir>

#include <QDebug>

GraphViewer::GraphViewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GraphViewer)
{
    ui->setupUi(this);

    this->setMinimumSize(400, 280);
    m_pixmaphView = new PixmapView;
    m_thumbnailBar = new ThumbnailBar;
    m_editPath = new QLineEdit("I:\\Yvi\\images");
    m_editPath->setStyleSheet("background:transparent; border:0;color:white;");

    QPushButton* btnGo = new QPushButton("Go");
    btnGo->setFixedSize(40, 21);
    QGridLayout* grid = new QGridLayout(m_pixmaphView);
    grid->setMargin(0);
    QHBoxLayout *hLayout = new QHBoxLayout;
    QSpacerItem* hSpacer = new QSpacerItem(10, 10,
                                            QSizePolicy::Expanding,
                                            QSizePolicy::Minimum);
    hLayout->addWidget(m_editPath);
    hLayout->addWidget(btnGo);
    hLayout->addItem(hSpacer);
    grid->addLayout(hLayout, 0, 0, 1, 2);

    QSpacerItem* verticalSpacer = new QSpacerItem(20, 40,
                                            QSizePolicy::Minimum,
                                            QSizePolicy::Expanding);
    grid->addItem(verticalSpacer, 1, 1, 1, 1);
    grid->addWidget(m_thumbnailBar, 2, 0, 1, 6);
    this->setCentralWidget(m_pixmaphView);

    connect(btnGo, SIGNAL(clicked()), this, SLOT(loadImages()));
    connect(m_thumbnailBar, SIGNAL(thumbnailChanged(QString)), this, SLOT(imageChanged(QString)));
}

GraphViewer::~GraphViewer()
{
    delete ui;
}

void GraphViewer::imageChanged(const QString &filename)
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
    m_pixmaphView->setImage(pix);
}

void GraphViewer::loadImages()
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

void GraphViewer::resizeEvent(QResizeEvent *event)
{
    m_thumbnailBar->hide();
    QMainWindow::resizeEvent(event);
}

void GraphViewer::closeEvent(QCloseEvent *)
{
    m_thumbnailBar->reset();
    m_pixmaphView->reset();
    this->close();
}
