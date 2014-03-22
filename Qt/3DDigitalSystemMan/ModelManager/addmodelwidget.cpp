#include "addmodelwidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPalette>
#include <QBitmap>
#include <QPainter>
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QDir>
#include <QFileDialog>
#include <QUrl>
#include <QFileInfo>
#include <QCursor>
#include <QToolTip>

#include <QDebug>

AddModelWidget::AddModelWidget(QWidget *parent) :
    QWidget(parent)
{
    this->setAttribute(Qt::WA_DeleteOnClose);//ϣ���ô������˳�ʱ�Զ��ܹ�delete���Լ�
    this->setAutoFillBackground(true);
    this->setStyleSheet(0);
    this->setGeometry(QRect(2, -220, 400, 220));

    initializWindow();
    setDefautData();

    this->m_ledtFileDir->setAcceptDrops(true);
    this->m_ledtFileDir->installEventFilter(this);
    this->m_lbThumb->setAcceptDrops(true);
    this->m_lbThumb->installEventFilter(this);

    connect( m_pbtnOk, SIGNAL(clicked()), this, SLOT(OnOkAddModelWidgetSlot()) );
    connect( m_pbtnCancel, SIGNAL(clicked()), this, SLOT(OnCancelAddModelWidgetSlot()) );
    connect( m_pbtnSelectFile, SIGNAL(clicked()), this, SLOT(OnSelModelFileBtnSlot()) );

    //add model
    connect( this, SIGNAL(addMaxModelSignal(const QString&, const QString&, const QString&, const QString&)),
             this->parentWidget()->window(), SLOT(OnAddMaxModelSlot(const QString&, const QString&, const QString&, const QString&)) );
}

/* create and initalize add model widget controls  */
void AddModelWidget::initializWindow()
{
    QGridLayout *gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(6);
    //gridLayout->setContentsMargins(0, 0, 0, 0);
    //gridLayout->setGeometry(QRect(10, 10, 380, 220));

    QLabel *lbFile = new QLabel(tr("�ļ�:"),this);
    QLabel *lbType = new QLabel(tr("����:"),this);
    QLabel *lbVersion = new QLabel(tr("�汾:"),this);
    QLabel *lbMat = new QLabel(tr("����:"),this);
    QLabel *lbName = new QLabel(tr("����:"),this);
    QLabel *lbRemark = new QLabel(tr("��ע:"),this);

    m_ledtFileDir       = new QLineEdit(this);
    m_pbtnSelectFile  = new QPushButton(tr("..."),this);
    m_ledtType       = new QLineEdit(this);
    m_ledtVersion    = new QLineEdit(this);
    m_ledtHasMat     = new QLineEdit(this);
    m_pbtnAddMat      = new QPushButton(tr("�����ͼ"),this);
    m_ledtName          = new QLineEdit(this);
    m_edtRemark       = new QTextEdit(this);
    m_lbThumb         = new QLabel(tr("Thumbnails"),this);
    m_pbtnOk = new QPushButton(tr("ȷ��"), this);
    m_pbtnCancel = new QPushButton(tr("ȡ��"), this);

    gridLayout->addWidget(lbFile,       0, 0, 1, 1);
    gridLayout->addWidget(lbType,       1, 0, 1, 1);
    gridLayout->addWidget(lbVersion,    2, 0, 1, 1);
    gridLayout->addWidget(lbMat,        3, 0, 1, 1);
    gridLayout->addWidget(lbName,       4, 0, 1, 1);
    gridLayout->addWidget(lbRemark,     5, 0, 1, 1);
    gridLayout->addWidget(m_ledtType,          1, 1, 1, 1);
    gridLayout->addWidget(m_ledtVersion,       2, 1, 1, 1);
    gridLayout->addWidget(m_ledtHasMat,        3, 1, 1, 1);
    gridLayout->addWidget(m_ledtName,             4, 1, 1, 1);

    gridLayout->addWidget(m_ledtFileDir,          0, 1, 1, 6);
    gridLayout->addWidget(m_pbtnSelectFile, 0, 7, 1, 1);

    QSpacerItem* verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    gridLayout->addItem(verticalSpacer, 6, 0, 1, 1);

    gridLayout->addWidget(m_lbThumb,    1, 3, 5, 5);
    gridLayout->addWidget(m_edtRemark,  5, 1, 2, 2);
    gridLayout->addWidget(m_pbtnAddMat, 3, 2, 1, 1);
    QSpacerItem* horizontalSpacer_0 = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    gridLayout->addItem(horizontalSpacer_0, 6, 3, 1, 5);


    gridLayout->addWidget(m_pbtnOk,             7, 6, 1, 1);
    gridLayout->addWidget(m_pbtnCancel,         7, 7, 1, 1);


    QSpacerItem* horizontalSpacer_1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    gridLayout->addItem(horizontalSpacer_1, 1, 2, 1, 1);
    QSpacerItem* horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    gridLayout->addItem(horizontalSpacer_2, 2, 2, 1, 1);
    QSpacerItem* horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    gridLayout->addItem(horizontalSpacer_3, 4, 2, 1, 1);
    QSpacerItem* horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    gridLayout->addItem(horizontalSpacer_4, 7, 0, 1, 1);
    QSpacerItem* horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    gridLayout->addItem(horizontalSpacer_5, 7, 1, 1, 5);

    this->setLayout(gridLayout);



}

/* reset add max model widget data to empty */
void AddModelWidget::clearData()
{
    m_ledtFileDir->clear();
    m_ledtName->clear();
    m_edtRemark->clear();
    m_lbThumb->clear();
    m_ledtType->clear();
    setDefautData();
}

/* reset add max model widget data to default */
void AddModelWidget::setDefautData()
{
    m_lbThumb->setFixedSize(158, 148);
    QString thumbFile = QString(":/images/nopic.jpg");
    m_lbThumb->setPixmap(QPixmap(thumbFile));
    m_lbThumb->setScaledContents(true);//Ϊ����QLabel�Զ����ţ��ȣ���ʾͼ���С�Զ�����ΪQlabel��С��
    m_ledtHasMat->setText(tr("����ͼ"));
}

/* show the widget window which add max model */
void AddModelWidget::OnOkAddModelWidgetSlot()
{
    setModelName(m_ledtName->text());
    setModelType(m_ledtType->text());
    if( m_modelFilename.isEmpty() )
    {
        QMessageBox::information(this, tr("��ʾ"), tr("��ָ��\"ģ���ļ�\""), QMessageBox::Ok);
        return;
    }
    if(m_modelType.isEmpty())
    {
        QMessageBox::information(this, tr("��ʾ"), tr("��ѡ��\"ģ�ͷ���\""), QMessageBox::Ok);
        return;
    }
    if(m_modelThumbname.isEmpty())
    {
        m_modelThumbname = ":/images/nopic.jpg";
    }
    if(m_modelName.isEmpty())
    {
        QMessageBox::information(this, tr("��ʾ"), tr("������\"ģ������\""), QMessageBox::Ok);
        return;
    }

    emit hideAddModelWgtSignal();
    //QString &modelType, const QString &maxFilename, const QString &thumbFilename, const QString &itemText

    addMaxModelSignal(m_modelType, m_modelFilename, m_modelThumbname, m_modelName);
}

/* hide the widget window which add max model */
void AddModelWidget::OnCancelAddModelWidgetSlot()
{
    emit hideAddModelWgtSignal();
    clearData();
}

/*add max file button*/
void AddModelWidget::OnSelModelFileBtnSlot()
{
    QString iniPath;
    if( !m_ledtFileDir->text().isEmpty() )
    {
        QDir dir(m_ledtFileDir->text());
        if( dir.exists() )
            iniPath = m_ledtFileDir->text();
        else
        {
            QFileInfo file(m_ledtFileDir->text());
            if(file.exists())
                iniPath = file.path();
        }
    }
    else
        iniPath = QDir::currentPath();
    QString filename = QFileDialog::getOpenFileName(
       this,
       "Open 3ds Max File",
       iniPath,
       "3ds Max(*.max *.MAX);;All files(*.*)");
    if (!filename.isNull())
    {
        m_ledtFileDir->setText(filename);
        setModelFilename(filename);
    }
}

void AddModelWidget::OnSetModelTypeSlot(const QString &category)
{
    setModelType(category);
    this->m_ledtType->setText(category);
}

/*set max model thumbnail*/
void AddModelWidget::setModelThumbnail()
{
    QString filename = QFileDialog::getOpenFileName(
       this,
       "Open Picture",
       QDir::currentPath(),
       "Thumbnail files (*.jpg *.png *.bmp *.gif);;All files(*.*)");
    if (!filename.isEmpty())
    {
        m_lbThumb->setPixmap(QPixmap(filename));
        m_modelThumbname = filename;
    }
}

/*
 * drop picture nad max file to thumbnail label and file path lineedit
 *
*/
bool AddModelWidget::eventFilter(QObject *obj, QEvent *event)
{
    QRect labelRect = QRect( QPoint(0,0), m_lbThumb->size() );
    switch( event->type() )
    {
    case QEvent::MouseButtonRelease:
    {
        if( obj == m_lbThumb )
        {

            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if( mouseEvent->button() == Qt::LeftButton && labelRect.contains(mouseEvent->pos()) )
            {
                setModelThumbnail();
                return true;
            }
        }
    };break;

    case QEvent::Enter:
    {
        if( obj == m_lbThumb )
        {
            QToolTip::showText(QCursor::pos(), tr("����ѡ��ͼƬ/��ͼƬ�������ͼ"), this);
            this->m_lbThumb->setCursor(Qt::PointingHandCursor);
        }
        else
        {
            QToolTip::showText(QCursor::pos(), tr("֧����קmax�ļ����"), this);
        }
    };break;

    case QEvent::Leave:
    {
        if( obj == m_lbThumb )
        {
            this->setCursor(Qt::ArrowCursor);
        }
    };break;

    case QEvent::Drop:
    {
        QDropEvent *dropEvent = static_cast<QDropEvent*>(event);
        if( obj == m_lbThumb )
        {
            QString extRx = "\\.(jpg|png|bmp|gif)$";
            QString filename = "";
            fileDrop(dropEvent, extRx, filename);
            this->m_modelThumbname = filename;
            m_lbThumb->clear();
            m_lbThumb->setPixmap( QPixmap(m_modelThumbname) );
        }
        else if( obj == m_ledtFileDir )
        {
            QString extRx = "\\.(max|MAX)$";
            QString filename = "";
            fileDrop(dropEvent, extRx, filename);
            this->m_modelThumbname = filename;
            this->m_ledtFileDir->clear();
            this->m_ledtFileDir->setText(m_modelThumbname);
        }
    };break;

    case QEvent::DragEnter:
    {
        QDragEnterEvent *drapEvent = static_cast<QDragEnterEvent*>(event);
        if( obj == m_lbThumb )
        {
            QString extRx = "\\.(jpg|png|bmp|gif)$";
            fileDragEnter(drapEvent, extRx);
        }
        else if( obj == m_ledtFileDir )
        {
            QString extRx = "\\.(max|MAX)$";
            fileDragEnter(drapEvent, extRx);
        }
    };break;

    default:break;
    }//switch

    // pass the event on to the parent class
    return QWidget::eventFilter(obj, event);
}

//like:"\\.(jpg|png|bmp|gif)$"
void AddModelWidget::fileDrop(QDropEvent *event, const QString &dragTypeRx, QString &fullFilename)
{
    if( event->mimeData()->hasUrls() )
    {
        QRegExp rx( dragTypeRx, Qt::CaseInsensitive );
        QString localFile;
        foreach(QUrl url, event->mimeData()->urls())
        {
            localFile = url.toLocalFile();
            if( rx.indexIn(localFile) >= 0 )
            {
                event->accept();
                event->setDropAction(Qt::CopyAction);
                //set label picture
                QFileInfo fileInfo(localFile);
                fullFilename = fileInfo.absoluteFilePath();
            }
        }
    }
}

//"\\.(jpg|png|bmp|gif)$"
void AddModelWidget::fileDragEnter(QDragEnterEvent *event, const QString &dragTypeRx)
{
    if (event->mimeData()->hasFormat("text/uri-list"))
    {
        QString localFile;
        QRegExp rx( dragTypeRx, Qt::CaseInsensitive );
        foreach(QUrl url, event->mimeData()->urls())
        {
            localFile = url.toLocalFile();
            if( rx.indexIn(localFile) >= 0 )
            {
                event->acceptProposedAction();
            }
            else
                event->ignore();
        }
    }
}

