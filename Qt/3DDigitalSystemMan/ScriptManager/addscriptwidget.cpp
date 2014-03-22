#include "addscriptwidget.h"
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

AddScriptWidget::AddScriptWidget(QWidget *parent) :
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

    connect( m_pbtnOk, SIGNAL(clicked()), this, SLOT(OnOkAddScriptWidgetSlot()) );
    connect( m_pbtnCancel, SIGNAL(clicked()), this, SLOT(OnCancelAddScriptWidgetSlot()) );
    connect( m_pbtnSelectFile, SIGNAL(clicked()), this, SLOT(OnSelScriptFileBtnSlot()) );

    //add Script
    connect( this, SIGNAL(addScriptSignal(const QString&, const QString&, const QString&, const QString&)),
             this->parentWidget()->window(), SLOT(OnAddScriptSlot(const QString&, const QString&, const QString&, const QString&)) );
}

/* create and initalize add Script widget controls  */
void AddScriptWidget::initializWindow()
{
    QGridLayout *gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(6);
    //gridLayout->setContentsMargins(0, 0, 0, 0);
    //gridLayout->setGeometry(QRect(10, 10, 380, 220));

    QLabel *lbFile = new QLabel(tr("�ű�:"),this);
    QLabel *lbType = new QLabel(tr("����:"),this);
    QLabel *lbVersion = new QLabel(tr("�汾:"),this);
    //QLabel *lbMat = new QLabel(tr("����:"),this);
    QLabel *lbName = new QLabel(tr("����:"),this);
    QLabel *lbRemark = new QLabel(tr("��ע:"),this);

    m_ledtFileDir       = new QLineEdit(this);
    m_pbtnSelectFile  = new QPushButton(tr("..."),this);
    m_ledtType       = new QLineEdit(this);
    m_ledtVersion    = new QLineEdit(this);
    //m_ledtHasMat     = new QLineEdit(this);
    //m_pbtnAddMat      = new QPushButton(tr("�����ͼ"),this);
    m_ledtName          = new QLineEdit(this);
    m_edtRemark       = new QTextEdit(this);
    m_lbThumb         = new QLabel(tr("Thumbnails"),this);
    m_pbtnOk = new QPushButton(tr("ȷ��"), this);
    m_pbtnCancel = new QPushButton(tr("ȡ��"), this);

    gridLayout->addWidget(lbFile,       0, 0, 1, 1);
    gridLayout->addWidget(lbType,       1, 0, 1, 1);
    gridLayout->addWidget(lbVersion,    2, 0, 1, 1);
    //gridLayout->addWidget(lbMat,        3, 0, 1, 1);
    gridLayout->addWidget(lbName,       3, 0, 1, 1);
    gridLayout->addWidget(lbRemark,     4, 0, 1, 1);
    gridLayout->addWidget(m_ledtType,          1, 1, 1, 1);
    gridLayout->addWidget(m_ledtVersion,       2, 1, 1, 1);
    //gridLayout->addWidget(m_ledtHasMat,        3, 1, 1, 1);
    gridLayout->addWidget(m_ledtName,             3, 1, 1, 1);

    gridLayout->addWidget(m_ledtFileDir,          0, 1, 1, 6);
    gridLayout->addWidget(m_pbtnSelectFile, 0, 7, 1, 1);

    QSpacerItem* verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    gridLayout->addItem(verticalSpacer, 5, 0, 1, 1);

    gridLayout->addWidget(m_lbThumb,    1, 4, 5, 5);
    gridLayout->addWidget(m_edtRemark,  4, 1, 4, 2);
    //gridLayout->addWidget(m_pbtnAddMat, 3, 2, 1, 1);

    QSpacerItem* horizontalSpacer_0 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    gridLayout->addItem(horizontalSpacer_0, 2, 2, 1, 1);


    gridLayout->addWidget(m_pbtnOk,             8, 6, 1, 1);
    gridLayout->addWidget(m_pbtnCancel,         8, 7, 1, 1);


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
    QSpacerItem* horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    gridLayout->addItem(horizontalSpacer_6, 8, 1, 1, 5);
    QSpacerItem* horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    gridLayout->addItem(horizontalSpacer_7, 9, 1, 1, 5);

    this->setLayout(gridLayout);
}

/* reset add max Script widget data to empty */
void AddScriptWidget::clearData()
{
    m_ledtFileDir->clear();
    m_ledtName->clear();
    m_edtRemark->clear();
    m_lbThumb->clear();
    m_ledtType->clear();
    setDefautData();
}

/* reset add max Script widget data to default */
void AddScriptWidget::setDefautData()
{
    m_lbThumb->setFixedSize(158, 148);
    QString thumbFile = QString(":/images/nopic.jpg");
    m_lbThumb->setPixmap(QPixmap(thumbFile));
    m_lbThumb->setScaledContents(true);//Ϊ����QLabel�Զ����ţ��ȣ���ʾͼ���С�Զ�����ΪQlabel��С��
    //m_ledtHasMat->setText(tr("����ͼ"));
}

/* show the widget window which add max Script */
void AddScriptWidget::OnOkAddScriptWidgetSlot()
{
    setScriptName(m_ledtName->text());
    setScriptType(m_ledtType->text());
    if( m_scriptFilename.isEmpty() )
    {
        QMessageBox::information(this, tr("��ʾ"), tr("��ָ��\"�ű��ļ�\""), QMessageBox::Ok);
        return;
    }
    if(m_scriptType.isEmpty())
    {
        QMessageBox::information(this, tr("��ʾ"), tr("��ѡ��\"�ű�����\""), QMessageBox::Ok);
        return;
    }
    if(m_scriptThumbname.isEmpty())
    {
        m_scriptThumbname = ":/images/nopic.jpg";
    }
    if(m_scriptName.isEmpty())
    {
        QMessageBox::information(this, tr("��ʾ"), tr("������\"�ű�����\""), QMessageBox::Ok);
        return;
    }

    emit hideScriptWgtSignal();
    //QString &ScriptType, const QString &maxFilename, const QString &thumbFilename, const QString &itemText

    addScriptSignal(m_scriptType, m_scriptFilename, m_scriptThumbname, m_scriptName);
}

/* hide the widget window which add max Script */
void AddScriptWidget::OnCancelAddScriptWidgetSlot()
{
    emit hideScriptWgtSignal();
    clearData();
}

/*add max file button*/
void AddScriptWidget::OnSelScriptFileBtnSlot()
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
       "Open 3dsMax MaxSript File",
       iniPath,
       "MaxScript (*.ms *.MS);;MaxEncrypt (*.mse *.MSE);;MaxScript zip (*.mzp *.MZP);;All files(*.*)");
    if (!filename.isNull())
    {
        m_ledtFileDir->setText(filename);
        setScriptFilename(filename);
    }
}

void AddScriptWidget::OnSetScriptTypeSlot(const QString &category)
{
    setScriptType(category);
    this->m_ledtType->setText(category);
}

/*set max Script thumbnail*/
void AddScriptWidget::setScriptThumbnail()
{
    QString filename = QFileDialog::getOpenFileName(
       this,
       "Open Picture",
       QDir::currentPath(),
       "Thumbnail files (*.jpg *.png *.bmp *.gif);;All files(*.*)");
    if (!filename.isEmpty())
    {
        m_lbThumb->setPixmap(QPixmap(filename));
        m_scriptThumbname = filename;
    }
}

/*
 * drop picture nad max file to thumbnail label and file path lineedit
 *
*/
bool AddScriptWidget::eventFilter(QObject *obj, QEvent *event)
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
                setScriptThumbnail();
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
            QToolTip::showText(QCursor::pos(), tr("֧����קms/mse/mzp�ű��ļ����"), this);
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
            this->m_scriptThumbname = filename;
            m_lbThumb->clear();
            m_lbThumb->setPixmap( QPixmap(m_scriptThumbname) );
        }
        else if( obj == m_ledtFileDir )
        {
            QString extRx = "\\.(ms|mse|mzp)$";
            QString filename = "";
            fileDrop(dropEvent, extRx, filename);
            this->m_scriptThumbname = filename;
            this->m_ledtFileDir->clear();
            this->m_ledtFileDir->setText(m_scriptThumbname);
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
            QString extRx = "\\.(ms|mse|mzp)$";
            fileDragEnter(drapEvent, extRx);
        }
    };break;

    default:break;
    }//switch

    // pass the event on to the parent class
    return QWidget::eventFilter(obj, event);
}

//like:"\\.(jpg|png|bmp|gif)$"
void AddScriptWidget::fileDrop(QDropEvent *event, const QString &dragTypeRx, QString &fullFilename)
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
void AddScriptWidget::fileDragEnter(QDragEnterEvent *event, const QString &dragTypeRx)
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

